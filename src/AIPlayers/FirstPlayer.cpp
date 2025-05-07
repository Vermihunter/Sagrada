#include "FirstPlayer.h"

FirstPlayer::FirstPlayer(const AIPlayerContext& _ctx) : AI_Player(_ctx) {}

ID_t FirstPlayer::choose_window_pattern_card(wpc_it_t iBegin, wpc_it_t iEnd)
{
	return 0;
}

move_t FirstPlayer::make_next_move()
{
	return ctx.game.get_single_move(Game::SINGLE_MOVE_STRATEGY_FIRST);
}

FirstPlayer::~FirstPlayer()
{
	// dtor
}
