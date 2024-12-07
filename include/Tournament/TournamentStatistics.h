#ifndef TOURNAMENT_STATISTICS_H
#define TOURNAMENT_STATISTICS_H

#include <vector>

#include "AI_Player.h"
#include "PlayerStatistics.h"
#include "Typedefs.h"
#include "WilsonScoreInterval.h"

/**
 * @brief Result of a game ran by the tournament
 */
struct GameResult
{
    GameResult() : winner(nullptr) {}

    std::vector<std::pair<AI_PlayerConfig*, int>> pointsByPlayers;
    AI_PlayerConfig* winner;
};

using game_result_t = std::unique_ptr<GameResult>;

/**
 * @brief All data that is remembered about the result of a tournament
 */
struct TournamentStatistics
{
    std::unordered_map<AI_PlayerConfig*, PlayerStatistics> playerStatistics;

    TournamentStatistics(ai_player_config_c& configs)
    {
        for (auto&& config : configs) {
            playerStatistics.insert({config.get(), PlayerStatistics {*this}});
        }
    }

    std::vector<game_result_t> gameResults;

    AI_PlayerConfig* total_winner() const
    {
        const auto compareWins = [](const auto& player1, const auto& player2) {
            return player1.second < player2.second;
        };

        std::unordered_map<AI_PlayerConfig*, unsigned int> winByPlayers;

        const auto getWinner = [&](const game_result_t& gameResult) {
            if (!gameResult->winner) {
                return;
            }

            AI_PlayerConfig* winner = gameResult->winner;
            if (winByPlayers.find(winner) == winByPlayers.end()) {
                winByPlayers[winner] = 1;
            }
            else {
                ++winByPlayers.at(winner);
            }
        };

        std::for_each(gameResults.begin(), gameResults.end(), getWinner);
        return std::max_element(winByPlayers.begin(), winByPlayers.end(),
                                compareWins)
            ->first;
    }
};

#endif // TOURNAMENT_STATISTICS_H