#include "RulesBasedPlayerConfig.h"

RulesBasedPlayerConfig::RulesBasedPlayerConfig(RulesBasedPlayer::strategy_t _strategy)
    : strategy(_strategy)
{
	name = type + "_";
	if (strategy == RulesBasedPlayer::ALL_MOVES)
		name += "ALLMOVES";
	else if (strategy == RulesBasedPlayer::ONLY_DTFM)
		name += "ONLYDTFM";
}

ai_player_t RulesBasedPlayerConfig::create_player(const AIPlayerContext& _ctx) const
{
	return std::make_unique<RulesBasedPlayer>(_ctx, strategy);
}