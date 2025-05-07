#include "MinimaxPlayerConfig.h"

#include "MinimaxPlayer.h"

MinimaxPlayerConfig::MinimaxPlayerConfig(size_t _depth, const HeuristicConstants& _constants,
                                         int _worldCount)
    : depth(_depth), constants(_constants), worldCount(_worldCount)
{
	nameWithDepth = type + std::to_string(depth) +
	                (_worldCount > 0 ? ",worlds=" + std::to_string(worldCount) : "");
}

ai_player_t MinimaxPlayerConfig::create_player(const AIPlayerContext& _ctx) const
{
	if (!_ctx.gameDeterministic && worldCount <= 0) {
		throw std::invalid_argument{"In a non-deterministic world, the world "
		                            "count has to be a positive integer!"};
	}

	return std::make_unique<MinimaxPlayer>(_ctx, depth, constants, worldCount);
}