#ifndef MCTS_PLAYER_CONTEXT_H
#define MCTS_PLAYER_CONTEXT_H

#include "AI_PlayerConfig.h"
#include "MCTSPlayerDefaultConfig.h"
#include "Typedefs.h"

#include <memory>
#include <optional>

/**
 * @brief Represents the configurable parameters of the MCTS player
 *
 */
struct MCTSPlayerContext {
	MCTSPlayerContext() : explorationConstant(MCTS_defaultExplorationConstant) {}

	MCTSPlayerContext(const MCTSPlayerContext& other)
	    : iterationCount(other.iterationCount), playoutStrategy(other.playoutStrategy->clone()),
	      determinizingWorldCount(other.determinizingWorldCount),
	      explorationConstant(other.explorationConstant)
	{
	}

	auto clone() const { return std::make_unique<MCTSPlayerContext>(*this); }

	size_t                iterationCount;
	ai_player_config_t    playoutStrategy;
	std::optional<size_t> determinizingWorldCount;
	double                explorationConstant;
};

using mcts_context_t = std::unique_ptr<MCTSPlayerContext>;

#endif // MCTS_PLAYER_CONTEXT_H