#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include "AI_PlayerConfig.h"
#include "Typedefs.h"

struct GameConfig
{
    int randomSeed;
    bool gameDeterministic;
    cached_game_ctx_builder_t gameCtxBuilder;
    ai_player_config_c players;
};

using game_config_t = std::unique_ptr<GameConfig>;

#endif // GAME_CONFIG_H