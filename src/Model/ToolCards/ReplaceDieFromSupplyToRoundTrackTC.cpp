#include "ReplaceDieFromSupplyToRoundTrackTC.h"

#include "Game.h"

ReplaceDieFromSupplyToRoundTrackTC::ReplaceDieFromSupplyToRoundTrackTC()
{
	// ctor
}

ToolCardUseFeedback ReplaceDieFromSupplyToRoundTrackTC::use(tool_card_move_t m, Game& g)
{
	auto mv = to_concrete_type<ReplaceDieFromSupplyToRoundTrackTC::Move>(m);

	replace_dice(mv, g);
	g.move_request(&mv->placingMove, /** allowDieToFieldMove = */ true);

	base_use_impl();
	return ToolCardUseFeedback{.shiftRound = true, .tcUsageFinished = true, .extractCost = true};
}

void ReplaceDieFromSupplyToRoundTrackTC::replace_dice(replace_move_t& mv, Game& g)
{
	g.get_ctx()->roundTrack.replace_die(mv->dieInSupply, mv->dieOnRoundTrack);
	if (mv->placingMove.die == mv->dieInSupply) {
		mv->placingMove.die = mv->dieOnRoundTrack;
	}
	else {
		mv->placingMove.die = mv->dieInSupply;
	}
}

void ReplaceDieFromSupplyToRoundTrackTC::append_possible_moves(ToolCardAppendContext& ctx)
{
	auto& dice            = ctx.g.get_curr_state()->currentRoundRemainingDice;
	auto& roundtrack_dice = ctx.g.get_ctx()->roundTrack.get_dice();

	for (auto&& die : dice) {
		for (auto&& rt_die : roundtrack_dice) {
			append_moves_for(die, rt_die, ctx);
		}
	}
}

void ReplaceDieFromSupplyToRoundTrackTC::append_moves_for(Die* currentRoundDie, Die* roundTrackDie,
                                                          ToolCardAppendContext& ctx)
{
	auto&          basicMoveHandler = ctx.g.get_ctx()->basicMoveHandler;
	DieToFieldMove placeMove{ctx.playerId, roundTrackDie, BoardIndex{}, DTFM_MODE_DEFAULT};
	for (auto&& possibleBoardInd :
	     basicMoveHandler->indices_to_put_to(ctx.g, ctx.playerId, roundTrackDie)) {
		// Not bug, putting the currentRoundDie in the move because they will be
		// swapped, but the pointer needs to point to a die from Game's
		// currentRoundDice
		placeMove.boardIndices = possibleBoardInd;
		ctx.moveContainer.push_back(new ReplaceDieFromSupplyToRoundTrackTC::Move(
		    ctx.playerId, ctx.toolCardInd, currentRoundDie, roundTrackDie, placeMove));
	}
}

ToolCardUseFeedback ReplaceDieFromSupplyToRoundTrackTC::undo(tool_card_move_t& m, Game& g)
{
	auto replaceMove = to_concrete_type<ReplaceDieFromSupplyToRoundTrackTC::Move>(m);

	g.undo_dtfm(&replaceMove->placingMove, true, true);
	// Reverse -> swapping for the second time will give the same result as
	// before the first swap
	replace_dice(replaceMove, g);

	base_undo_impl();
	return ToolCardUseFeedback{.shiftRound = false, .tcUsageFinished = true, .extractCost = true};
}

ReplaceDieFromSupplyToRoundTrackTC::~ReplaceDieFromSupplyToRoundTrackTC()
{
	// dtor
}
