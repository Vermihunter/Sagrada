#include "MCTSPlayerConfig.h"

#include "MCTSPlayer.h"

#include <sstream>

MCTSPlayerConfig::MCTSPlayerConfig(mcts_context_t _ctx) : ctx(std::move(_ctx))
{
	std::ostringstream oss;
	oss << type << ctx->iterationCount << ",expl=" << std::fixed << std::setprecision(2)
	    << ctx->explorationConstant;
	if (ctx->determinizingWorldCount.has_value()) {
		oss << ",worlds=" << ctx->determinizingWorldCount.value();
	}
	oss << ",playout=" << ctx->playoutStrategy->get_name();

	name = oss.str();
}

MCTSPlayerConfig::MCTSPlayerConfig(const MCTSPlayerConfig& other)
    : ctx(other.ctx->clone()), name(other.name)
{
}

ai_player_t MCTSPlayerConfig::create_player(const AIPlayerContext& _ctx) const
{
	if (!_ctx.gameDeterministic && ctx->determinizingWorldCount <= 0) {
		throw std::invalid_argument{"If the game is not deterministic, there "
		                            "must be some worlds determined to use!"};
	}

	return std::make_unique<MCTSPlayer>(_ctx, ctx->clone());
}