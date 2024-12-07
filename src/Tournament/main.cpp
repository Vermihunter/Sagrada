#include <filesystem>
#include <fstream>
#include <memory>
#include <string>


#include "GameCacheInitializer.h"
#include "Date.h"
#include "DefinedAIPlayers.h"
#include "GameCache.h"
#include "GameContextBuilder.h"
#include "SagradaGameCMDLineParser.h"
#include "Tournament.h"
#include "TournamentCSVPrinter.h"
#include "TournamentConfig.h"
#include "TournamentStreamPrinter.h"

std::string generate_output_name_for_config(const tournament_config_t& config)
{
    std::ostringstream oss;
    oss << "tournament_results/Date_" << Date::get_curr_date() << "_Time_"
        << Date::get_curr_time() << "_"
        << "G_" << config->gameCount << "_S_" << config->gameConfig->randomSeed
        << "_P_" << config->gameConfig->players[0]->get_name();
    for (size_t i = 1; i < config->gameConfig->players.size(); ++i) {
        oss << "_VS_" << config->gameConfig->players[i]->get_name();
    }

    return oss.str();
}

tournament_config_t parse_config(int argc, char** argv)
{
    tournament_config_t tConfig = std::make_unique<TournamentConfig>();
    SagradaGameCMDLineParser tournamentParser(
        tConfig->logStream, true, nullptr, "build/tournament", "1.0",
        argparse::default_arguments::help);

    tournamentParser.add_argument("-b")
        .help("Runs the game with all permutations of players for all the "
              "random seeds e.g. for a 2 "
              "player game, both the players will play as player1 and player2.")
        .nargs(0)
        .default_value(false)
        .implicit_value(true);

    tournamentParser.add_description(
        "Runs a tournament on a single thread between the chosen AI players.\n"
        "\n" +
        tournamentParser.get_player_help());

    tournamentParser.add_argument("-csv")
        .help("An option to save the results into a directory with CSV files.")
        .implicit_value("default")
        .nargs(1);

    tournamentParser.add_argument("-ms")
        .help("An option to save additional statistics about the moves made by "
              "the player to the CSV "
              "results.")
        .nargs(0)
        .default_value(false)
        .implicit_value(true);

    tournamentParser.add_argument("-gi")
        .help("An option to save the game information like card indices to the "
              "CSV results.")
        .nargs(0)
        .default_value(false)
        .implicit_value(true);

    tournamentParser.add_epilog(
        "Examples:\n"
        "\n"
        "$ build/tournament -n 100 -p first mcts-it=150 -v\n"
        "$ build/tournament -n 200 -p minimax-depth=3 "
        "mcts-it=150,playout=rules-based -s 5555 -v\n"
        "$ build/tournament -n 1000 -p "
        "minimax-depth=3,config_file=minimax_config.json "
        "mcts-it=150,playout=minimax-depth=1 -v\n"
        "$ build/tournament -n 10000 -p random first -csv "
        "tournament_results/huge -v\n");

    try {
        tournamentParser.parse(argc, argv);
    }
    catch (const std::runtime_error& err) {
        exit_with_err_message(err.what(), tournamentParser);
    }

    GameCacheInitializer::initialize_cache();

    bool verbose = tournamentParser.get<bool>("-v");
    auto csvoutput = tournamentParser.present<std::string>("-csv");
    tConfig->gameConfig = tournamentParser.get_game_config();
    tConfig->allDirections = tournamentParser.get<bool>("-b");
    tConfig->saveMoveStatistics = tournamentParser.get<bool>("-ms");
    tConfig->saveGameInformation = tournamentParser.get<bool>("-gi");

    if (!verbose && !csvoutput) {
        exit_with_err_message("The tournament would not have any output "
                              "information! Specify -v and/or -csv",
                              tournamentParser);
    }

    if (!csvoutput && tConfig->saveMoveStatistics) {
        exit_with_err_message(
            "To save move statistics, the -csv option has to be defined",
            tournamentParser);
    }

    tConfig->gameCount = tournamentParser.get<unsigned int>("-n");
    if (tConfig->gameCount == 0) {
        exit_with_err_message("Game count has to be a positive integer!",
                              tournamentParser);
    }

    if (verbose) {
        ++tConfig->verboseLevel;
    }

    if (tConfig->verboseLevel >= 1) {
        tConfig->tournamentPrinters.push_back(
            std::make_unique<TournamentStreamPrinter>(*tConfig, std::cout));
    }

    if (csvoutput) {
        tConfig->output = *csvoutput;
        // Construct csv headers and config
        std::vector<std::string> csvGameResultsHeader {"seed"};
        for (size_t i = 0; i < tConfig->gameConfig->players.size(); ++i) {
            const std::string playerCountStr = std::to_string(i + 1);
            csvGameResultsHeader.push_back("Player " + playerCountStr);
            csvGameResultsHeader.push_back("Player " + playerCountStr +
                                           " score");
        }
        csvGameResultsHeader.push_back("winner");
        if (tConfig->saveMoveStatistics || tConfig->saveGameInformation) {
            csvGameResultsHeader.push_back("TC1");
            csvGameResultsHeader.push_back("TC2");
            csvGameResultsHeader.push_back("TC3");
            csvGameResultsHeader.push_back("PUOC1");
            csvGameResultsHeader.push_back("PUOC2");
            csvGameResultsHeader.push_back("PUOC3");
        }

        tConfig->tournamentPrinters.push_back(
            std::make_unique<TournamentCSVPrinter>(
                *tConfig, TournamentCSVPrinterConfig::create_default(
                              tConfig->output, csvGameResultsHeader)));

        const std::string csvOutputDir =
            (*csvoutput == "default") ? generate_output_name_for_config(tConfig)
                                      : *csvoutput;
        tConfig->tournamentPrinters.push_back(
            std::make_unique<TournamentCSVPrinter>(
                *tConfig, TournamentCSVPrinterConfig::create_default(
                              csvOutputDir, csvGameResultsHeader)));
    }
    return tConfig;
}

int main(int argc, char** argv)
{
    tournament_config_t tConfig = parse_config(argc, argv);
    Tournament::run(std::move(tConfig));
}
