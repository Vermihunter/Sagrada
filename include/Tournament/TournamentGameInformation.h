#ifndef TOURNAMENT_GAME_INFORMATION_H
#define TOURNAMENT_GAME_INFORMATION_H

#include <memory>

#include "EvalState.h"
#include "GameConfig.h"
#include "GameContextBuilder.h"
#include "OnlyAIPlayerController.h"
#include "Typedefs.h"

/**
 * @brief Information about a specific game of the tournament
 */
struct TournamentGameInformation
{
    TournamentGameInformation(const ai_player_config_pc& _aiConfigs)
        : aiConfigs(_aiConfigs)
    {
        gameInfo = std::make_unique<GameConfig>();
        for (auto&& aiConfig : _aiConfigs) {
            gameInfo->players.push_back(aiConfig->clone());
        }
    }

    static auto create(const ai_player_config_pc& _aiConfigs)
    {
        return std::make_unique<TournamentGameInformation>(_aiConfigs);
    }

    const ai_player_config_pc& aiConfigs;
    only_ai_controller_t controller;
    game_t game;
    game_config_t gameInfo;
    eval_state_c finalScores;
};

using tournament_game_info_t = std::unique_ptr<TournamentGameInformation>;

#endif // TOURNAMENT_GAME_INFORMATION_H