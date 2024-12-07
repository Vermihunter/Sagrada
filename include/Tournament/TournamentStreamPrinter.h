#ifndef TOURNAMENT_STREAM_PRINTER_H
#define TOURNAMENT_STREAM_PRINTER_H

#include <cmath>
#include <fstream>
#include <vector>

#include "ITournamentPrinter.h"
#include "TournamentConfig.h"

struct ColumnInformation
{
    size_t width;
    std::string colorHighlighter;
    std::string header;
};

// Define a custom manipulator for middle alignment
struct MiddleAlignManip
{
    int width;
    std::string text;

    MiddleAlignManip(int w, const std::string& t) : width(w), text(t) {}
};

// Overload the << operator for the custom manipulator
std::ostream& operator<<(std::ostream& os, const MiddleAlignManip& manip)
{
    int textWidth = manip.text.length();

    if (textWidth >= manip.width) {
        return os << manip.text;
    }

    int padding = (manip.width - textWidth);
    int rightPadding = padding / 2;
    int leftPadding = padding - rightPadding;
    return os << std::string(leftPadding, ' ') << manip.text
              << std::string(rightPadding, ' ');
}

// Helper function to create the custom manipulator
MiddleAlignManip middleAlign(int width, const std::string& text)
{
    return MiddleAlignManip(width, text);
}

// Define ANSI color codes
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

// Windows specific virtual terminal
#ifdef _WIN32
#    include <windows.h>

void enable_virtual_terminal()
{
    // Get the console handle
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) {
        std::cerr << "Error: Unable to get console handle" << std::endl;
        return;
    }

    // Get the current console mode
    DWORD mode = 0;
    if (!GetConsoleMode(hConsole, &mode)) {
        std::cerr << "Error: Unable to get console mode" << std::endl;
        return;
    }

    // Enable Virtual Terminal Processing
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hConsole, mode)) {
        std::cerr << "Error: Unable to set console mode" << std::endl;
        return;
    }
}

#else
// Dummy function for non-Windows platforms
void enable_virtual_terminal()
{
    // No action needed on non-Windows platforms
}
#endif

/**
 * @brief Prints a colorful log after each game to the console or another stream
 *
 */
class TournamentStreamPrinter : public ITournamentPrinter
{
  public:
    TournamentStreamPrinter(const TournamentConfig& _tournamentConfig,
                            std::ostream& _os)
        : ITournamentPrinter(_tournamentConfig)
        , os(_os)
        , gameCount(1)
        , rowWidth(0)
    {
        enable_virtual_terminal();
    }

    virtual void print_game_result(const TournamentGameInformation& gameInfo,
                                   game_result_t& result, int usedSeed) override
    {
        if (gameCount == 1) {
            print_header(result, usedSeed);
        }

        // Updating non constant columns
        gameResultColumns[0] = std::to_string(gameCount);
        gameResultColumns[1] = std::to_string(usedSeed);
        gameResultColumns[gameResultColumns.size() - 1] =
            (result->winner ? result->winner->get_name() : "No winner");
        for (size_t i = 0; i < tournamentConfig.gameConfig->players.size();
             ++i) {
            auto pointByPlayerIt = std::find_if(
                result->pointsByPlayers.begin(), result->pointsByPlayers.end(),
                [&](const std::pair<AI_PlayerConfig*, int>& scoreByPlayer) {
                    return scoreByPlayer.first == gameInfo.aiConfigs[i];
                });

            gameResultColumns[2 + i * 2 + 1] =
                std::to_string(pointByPlayerIt->second);
            gameResultColumns[2 + i * 2] = gameInfo.aiConfigs[i]->get_name();
        }

        os << '#';
        for (size_t i = 0; i < columns.size(); ++i) {
            os << columns[i].colorHighlighter << std::setw(columns[i].width - 1)
               << gameResultColumns[i] << defaultHighlighter << " #";
        }
        os << std::endl;

        ++gameCount;
    }

    virtual void print_move_statistics(const move_c& moves, Game& g,
                                       size_t gameNumber,
                                       AI_PlayerConfig* playerConfig) override
    {
    }

    virtual void print_overall_statistics(
        const TournamentStatistics& statistics) override
    {
        // player statistics
        size_t tmpRowWidth = rowWidth - 5 /* 5 * '#' */;
        size_t aiTypeColumnWidth = std::max(size_t(7), maxNameWidth) + 2;
        std::vector<ColumnInformation> playerStatisticsColumns {
            ColumnInformation {.width = aiTypeColumnWidth,
                               .colorHighlighter = YELLOW,
                               .header = "AI type"},
            ColumnInformation {
                .width = 11, .colorHighlighter = RED, .header = "Win count"},
            ColumnInformation {.width = 16,
                               .colorHighlighter = BLUE,
                               .header = "Average points"},
            ColumnInformation {.width =
                                   tmpRowWidth - 11 - aiTypeColumnWidth - 16,
                               .colorHighlighter = GREEN,
                               .header = "Average time per move in ms"}};
        print_header_row(playerStatisticsColumns, "Player statistics");

        os << std::setprecision(3);
        for (auto&& [aiConfig, aiStat] : statistics.playerStatistics) {
            os << '#' << playerStatisticsColumns[0].colorHighlighter
               << std::setw(playerStatisticsColumns[0].width - 1)
               << aiConfig->get_name() << defaultHighlighter << " #"
               << playerStatisticsColumns[1].colorHighlighter
               << std::setw(playerStatisticsColumns[1].width - 1)
               << aiStat.winCount << defaultHighlighter << " #"
               << playerStatisticsColumns[2].colorHighlighter
               << std::setw(playerStatisticsColumns[2].width - 1)
               << double(aiStat.totalReceivedPoints) /
                      double(statistics.gameResults.size())
               << defaultHighlighter << " #"
               << playerStatisticsColumns[3].colorHighlighter
               << std::setw(playerStatisticsColumns[3].width - 1)
               << double(aiStat.totalMoveTimeInMs) /
                      double(aiStat.totalMoveCount)
               << defaultHighlighter << " #" << std::endl;
        }

        // Game statistics
        tmpRowWidth += 2;
        std::vector<ColumnInformation> gameStatisticsColumns {
            ColumnInformation {.width = tmpRowWidth + 1,
                               .colorHighlighter = MAGENTA,
                               .header = "Tournament winner"},
        };
        print_header_row(gameStatisticsColumns, "Game statistics");
        auto totalWinner = statistics.total_winner();
        os << "#" << gameStatisticsColumns[0].colorHighlighter
           << middleAlign(gameStatisticsColumns[0].width,
                          totalWinner->get_name())
           << defaultHighlighter << "#" << std::endl;

        WilsonScoreInterval wilsonIterval =
            statistics.playerStatistics.at(totalWinner)
                .get_wilson_score_interval_with_98_confidence();

        std::ostringstream oss;
        oss << std::setprecision(3);
        os << "# " << RED;
        oss << totalWinner->get_name() << " player wins "
            << wilsonIterval.interval_low * 100 << "% - "
            << wilsonIterval.interval_high * 100
            << "% of games with 98% confidence level";
        os << middleAlign(rowWidth - 3, oss.str()) << defaultHighlighter << "#"
           << std::endl;

        os << std::string(rowWidth, '#') << std::endl;
    }

  private:
    std::ostream& os;
    size_t gameCount;

    size_t rowWidth;
    size_t gameCountWidth;
    size_t seedWidth;
    size_t maxNameWidth;
    std::vector<size_t> playerNameWidths;
    std::vector<size_t> playerScoreWidths;

    std::vector<ColumnInformation> columns;
    std::vector<std::string> gameResultColumns;

    inline static const std::string gameCountHighlighter = RED;
    inline static const std::string seedHighlighter = YELLOW;
    inline static const std::string defaultHighlighter = WHITE;
    inline static const std::string playerTypeHighlighter = BLUE;
    inline static const std::string playerScoreHighlighter = GREEN;
    inline static const std::string winnerHighlighter = MAGENTA;

    void print_header_row(const std::vector<ColumnInformation>& headerRow,
                          const std::string& title)
    {
        os << std::string(rowWidth, '#') << std::endl;
        os << "# " << CYAN << middleAlign(rowWidth - 3, title)
           << defaultHighlighter << "#" << std::endl;
        os << std::string(rowWidth, '#') << std::endl;

        os << '#';
        for (auto&& column : headerRow) {
            os << column.colorHighlighter
               << middleAlign(column.width, column.header) << defaultHighlighter
               << "#";
        }
        os << std::endl << std::string(rowWidth, '#') << std::endl;
    }

    void print_header(const game_result_t& result, int usedSeed)
    {
        columns.push_back(
            ColumnInformation {.width = 12,
                               .colorHighlighter = gameCountHighlighter,
                               .header = "Game count"});
        columns.push_back(ColumnInformation {
            .width = size_t(std::max(
                4, std::max(get_number_of_digits(
                                tournamentConfig.gameConfig->randomSeed) +
                                2,
                            get_number_of_digits(
                                tournamentConfig.gameConfig->randomSeed +
                                tournamentConfig.gameCount)) +
                       2)),
            .colorHighlighter = seedHighlighter,
            .header = "Seed"});

        gameResultColumns.resize(
            3 + 2 * tournamentConfig.gameConfig->players.size());
        size_t ind = 1;
        maxNameWidth = 0;
        std::vector<size_t> AI_playerNameWidths;
        for (auto&& aiConfig : tournamentConfig.gameConfig->players) {
            size_t ai_playerNameWidth = aiConfig->get_name().size() + 2;
            AI_playerNameWidths.push_back(ai_playerNameWidth);
            maxNameWidth = std::max(maxNameWidth, ai_playerNameWidth);
        }

        if (tournamentConfig.allDirections) {
            for (auto&& AI_playerNameWidth : AI_playerNameWidths) {
                AI_playerNameWidth = maxNameWidth;
            }
        }

        for (auto&& aiConfig : tournamentConfig.gameConfig->players) {
            gameResultColumns[ind * 2] = aiConfig->get_name();
            std::string indStr = std::to_string(ind);
            size_t indWidth = indStr.size();

            columns.push_back(ColumnInformation {
                .width =
                    std::max(7 + 2 + indWidth, AI_playerNameWidths[ind - 1]),
                .colorHighlighter = playerTypeHighlighter,
                .header = "Player " + indStr});
            columns.push_back(
                ColumnInformation {.width = 6 + 2 + indWidth,
                                   .colorHighlighter = playerScoreHighlighter,
                                   .header = "Score " + indStr});
            ++ind;
        }

        columns.push_back(
            ColumnInformation {.width = std::max(size_t(9 + 2), maxNameWidth),
                               .colorHighlighter = winnerHighlighter,
                               .header = "Winner"});

        // First #
        rowWidth = 1;
        for (auto&& column : columns) {
            rowWidth += (column.width + 1);
        }

        print_header_row(columns, "Game results");
    }

    void print_centered(const std::string& str,
                        const ColumnInformation& columnInfo)
    {
        size_t padding = columnInfo.width - str.size();
        size_t leftPadding = padding / 2;
        size_t rightPadding = padding - leftPadding;

        os << std::string(leftPadding, ' ') << str << rightPadding;
    }

    int get_number_of_digits(size_t number)
    {
        return static_cast<int>(log10(number)) + 1;
    }
};

#endif // TOURNAMENT_STREAM_PRINTER_H