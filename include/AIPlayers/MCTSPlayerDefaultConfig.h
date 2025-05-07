#ifndef MCTS_PLAYER_DEFAULT_CONFIG_H
#define MCTS_PLAYER_DEFAULT_CONFIG_H

#include "RandomPlayerConfig.h"
#include "Typedefs.h"

#include <cmath>

inline constexpr int      MCTS_undeterministicWorldCount  = 4;
inline ai_player_config_t MCTS_defaultPlayoutStrategy     = std::make_unique<RandomPlayerConfig>();
inline const double       MCTS_defaultExplorationConstant = 0.6; // std::sqrt(2);

#endif // MCTS_PLAYER_DEFAULT_CONFIG_H