#ifndef TOURNAMENT_CSV_PRINTER_H
#define TOURNAMENT_CSV_PRINTER_H

#include "Debugger.h"
#include "HeuristicFilter.h"
#include "ITournamentPrinter.h"
#include "TournamentGameInformation.h"

#include <filesystem>
#include <fstream>
#include <iostream>

struct TournamentCSVPrinterConfig;
using tournament_csv_printer_config_t = std::unique_ptr<TournamentCSVPrinterConfig>;

namespace fs = std::filesystem;

/**
 * @brief Helper Tournament CSV config
 */
struct TournamentCSVPrinterConfig {
	static tournament_csv_printer_config_t create_default(const std::string& _outputDir,
	                                                      const std::vector<std::string>& _columns)
	{
		return std::make_unique<TournamentCSVPrinterConfig>(',', _outputDir, _columns);
	}

	TournamentCSVPrinterConfig() = default;
	TournamentCSVPrinterConfig(char _separator, const std::string& _outputDir,
	                           const std::vector<std::string>& _columns)
	    : separator(_separator), outputDir(_outputDir), columns(_columns)
	{
	}

	char                     separator;
	std::string              outputDir;
	std::vector<std::string> columns;
};

/**
 * @brief A specific printer for the tournament
 * This is the default way to store results for tournaments run by the
 * tournament_runner.py since it runs paralelly and it makes printing to console
 * not possible.
 *
 * It can also be used with the CLI.
 */
class TournamentCSVPrinter : public ITournamentPrinter {
  public:
	TournamentCSVPrinter(const TournamentConfig&         _tournamentConfig,
	                     tournament_csv_printer_config_t _config)
	    : ITournamentPrinter(_tournamentConfig), config(std::move(_config)),
	      gameResultOutputStream((const char*)(nullptr)),
	      overallGameStatisticsOutputStream((const char*)(nullptr))
	{
		fs::create_directories(config->outputDir);
		gameResultOutputStream.open(fs::path(config->outputDir) / fs::path("game_results.csv"));
		overallGameStatisticsOutputStream.open(fs::path(config->outputDir) /
		                                       fs::path("game_statistics.csv"));
		playerStatistcsStream.open(fs::path(config->outputDir) / fs::path("player_statistics.csv"));

		if (tournamentConfig.saveMoveStatistics) {
			moveStatisticsOutputStream = std::make_unique<std::ofstream>(
			    fs::path(config->outputDir) / fs::path("move_statistics.csv"));
			writeToCSV(*moveStatisticsOutputStream, "GameNumber", "RoundNumber", "PlayerType",
			           "DTFM_Count", "TC_1_MoveCount", "TC_2_MoveCount", "TC_3_MoveCount",
			           "TotalMoveCount", "FilteredTotalMoveCount");
		}

		print_row(gameResultOutputStream, config->columns);
	}

	virtual void print_move_statistics(const move_c& moves, Game& g, size_t gameNumber,
	                                   AI_PlayerConfig* playerConfig) override
	{
		size_t dtfmCount      = 0;
		size_t tcMoveCount[3] = {0, 0, 0};

		for (auto&& move : moves) {
			if (move->t == Move::type::DIE_TO_FIELD_MOVE) {
				++dtfmCount;
			}
			else if (move->t == Move::type::TOOL_CARD) {
				++tcMoveCount[((tool_card_move_t)(move))->ind];
			}
		}

		move_by_index_c bestMoves;
		move_by_index_c backupMoves;
		filter_moves(g, moves, bestMoves, backupMoves);

		print_all_moves(moves, playerConfig->get_name() + " moves");
		writeToCSV(*moveStatisticsOutputStream, gameNumber, g.get_curr_state()->roundNumber,
		           std::quoted(playerConfig->get_name()), dtfmCount, tcMoveCount[0], tcMoveCount[1],
		           tcMoveCount[2], moves.size(),
		           bestMoves.empty() ? backupMoves.size() : bestMoves.size());
	}

	virtual void print_game_result(const TournamentGameInformation& gameInfo, game_result_t& result,
	                               int usedSeed) override
	{
		gameResultOutputStream << usedSeed;

		for (size_t i = 0; i < tournamentConfig.gameConfig->players.size(); ++i) {
			auto pointByPlayerIt =
			    std::find_if(result->pointsByPlayers.begin(), result->pointsByPlayers.end(),
			                 [&](const std::pair<AI_PlayerConfig*, int>& scoreByPlayer) {
				                 return scoreByPlayer.first == gameInfo.aiConfigs[i];
			                 });

			gameResultOutputStream << config->separator
			                       << std::quoted(gameInfo.aiConfigs[i]->get_name())
			                       << config->separator << pointByPlayerIt->second;
		}

		gameResultOutputStream << config->separator
		                       << (result->winner ? "\"" + result->winner->get_name() + "\""
		                                          : "No winner");

		if (tournamentConfig.saveGameInformation || tournamentConfig.saveMoveStatistics) {
			for (auto&& toolCard : gameInfo.game->get_ctx()->toolCards) {
				gameResultOutputStream << config->separator << size_t(toolCard->get_type());
			}

			for (auto&& puoc : gameInfo.game->get_ctx()->publicObjectiveCards) {
				gameResultOutputStream << config->separator << size_t(puoc->get_type());
			}
		}

		gameResultOutputStream << std::endl;
	}

	virtual void print_overall_statistics(const TournamentStatistics& statistics)
	{
		// player statistics
		writeToCSV(playerStatistcsStream, "AI type", "Win count", "Placement points",
		           "Average points", "Total move time in ms", "Total move count",
		           "Average move time in ms");
		for (auto&& [playerConfig, playerResults] : statistics.playerStatistics) {
			writeToCSV(playerStatistcsStream, "\"" + playerConfig->get_name() + "\"",
			           playerResults.winCount, playerResults.totalPlacementPoints,
			           ((double)(playerResults.totalReceivedPoints) /
			            (double)(statistics.gameResults.size())),
			           playerResults.totalMoveTimeInMs, playerResults.totalMoveCount,
			           (double)playerResults.totalMoveTimeInMs /
			               (double)playerResults.totalMoveCount);
		}

		// overall statistics
		auto winnerPlayer   = statistics.total_winner();
		auto wilsonInterval = statistics.playerStatistics.at(winnerPlayer)
		                          .get_wilson_score_interval_with_98_confidence();
		writeToCSV(overallGameStatisticsOutputStream, "Wilson interval lower bound in %",
		           "Wilson interval upper bound in %", "Wilson interval confidence level",
		           "Winner player type");
		overallGameStatisticsOutputStream << std::setprecision(3);
		writeToCSV(overallGameStatisticsOutputStream, wilsonInterval.interval_low * 100,
		           wilsonInterval.interval_high * 100, wilsonInterval.confidence,
		           "\"" + winnerPlayer->get_name() + "\"");
	}

  private:
	tournament_csv_printer_config_t config;
	std::ofstream                   gameResultOutputStream;
	std::ofstream                   playerStatistcsStream;
	std::ofstream                   overallGameStatisticsOutputStream;
	std::unique_ptr<std::ofstream>  moveStatisticsOutputStream;

	template <typename T> void writeToCSV(std::ofstream& file, const T& last)
	{
		file << last << std::endl;
	}

	// Recursive template function to write variables of different types to a
	// CSV file
	template <typename T, typename... Args>
	void writeToCSV(std::ofstream& file, const T& data, const Args&... args)
	{
		file << data << config->separator;
		writeToCSV(file, args...); // Recursively call with remaining arguments
	}

	void print_row(std::ofstream& os, const std::vector<std::string>& row)
	{
		if (!row.empty()) {
			os << row[0];
		}
		for (size_t i = 1; i < row.size(); ++i) {
			os << config->separator << row[i];
		}
		os << std::endl;
	}
};

#endif // TOURNAMENT_CSV_PRINTER_H