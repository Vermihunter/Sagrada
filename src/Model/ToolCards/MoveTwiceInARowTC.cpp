#include "MoveTwiceInARowTC.h"

#include "Game.h"
#include "GameInfo.h"

MoveTwiceInARowTC::MoveTwiceInARowTC()
{
	// ctor
}

ToolCardUseFeedback MoveTwiceInARowTC::use(tool_card_move_t m, Game& g)
{
	if (!g.get_curr_state()->is_curr_player_first_move_this_round()) {
		LOG_D("BadMoveRequest 12");
		throw BadMoveRequest{"You can use this card only in your first turn of the round!"};
	}

	auto moveTwiceMove = to_concrete_type<MoveTwiceInARowTC::Move>(m);

	auto  currPlayerId  = m->playerId;
	auto& currTurns     = g.get_curr_state()->currRoundInfo->turnInfos;
	auto  currPlayerInd = g.get_curr_state()->currPlayerIndex;

	moveTwiceMove->prevRoundInfo = currTurns;

	/**
	 * The player would move twice in a row anyways
	 */
	if (currPlayerId == currTurns[currPlayerInd + 1].playerId) {
		LOG_D("BadMoveRequest 13");
		throw BadMoveRequest{"The use of the tool card\nwould not have any effect!"};
	}

	// Next player after the current
	auto firstRotateElementIt = currTurns.begin() + currPlayerInd + 1;
	// Next occurence of the current player -> surely found because of previous
	// two assumptions
	auto secondElementIt =
	    std::find_if(firstRotateElementIt, currTurns.end(),
	                 [&](const TurnInfo& turnInfo) { return turnInfo.playerId == currPlayerId; });

	/**
	 * Right rotates by one the subvector starting from 1 after the current
	 * index to the index of the second occurrence of the current player
	 */
	std::rotate(firstRotateElementIt, secondElementIt, secondElementIt + 1);
	base_use_impl();

	// The round should not be shifted, but the card has been successfully used
	// -> signal
	return ToolCardUseFeedback{.shiftRound = false, .tcUsageFinished = true, .extractCost = true};
}

void MoveTwiceInARowTC::append_possible_moves(ToolCardAppendContext& ctx)
{
	/** This could be used only in the first round */
	if (!ctx.g.get_curr_state()->is_curr_player_first_move_this_round()) return;

	size_t currPlayerInd = ctx.g.get_curr_state()->currPlayerIndex;
	auto&  turnInfos     = ctx.g.get_curr_state()->currRoundInfo->turnInfos;
	ID_t   currPlayerId  = GameInfo::player_on_move(ctx.g)->ID;
	/** Safe because its sure that its the first move of a player */
	ID_t nextPlayerId = turnInfos[currPlayerInd + 1].playerId;

	/** Tool card would not have any effect! */
	if (currPlayerId == nextPlayerId) return;

	ctx.moveContainer.push_back(new MoveTwiceInARowTC::Move(ctx.playerId, ctx.toolCardInd));
}

ToolCardUseFeedback MoveTwiceInARowTC::undo(tool_card_move_t& m, Game& g)
{
	auto moveTwiceMove                           = to_concrete_type<MoveTwiceInARowTC::Move>(m);
	g.get_curr_state()->currRoundInfo->turnInfos = moveTwiceMove->prevRoundInfo;

	base_undo_impl();
	return ToolCardUseFeedback{.shiftRound = false, .tcUsageFinished = true, .extractCost = true};
}

MoveTwiceInARowTC::~MoveTwiceInARowTC()
{
	// dtor
}
