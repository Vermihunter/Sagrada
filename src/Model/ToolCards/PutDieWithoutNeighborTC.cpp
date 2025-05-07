#include "PutDieWithoutNeighborTC.h"

#include "Game.h"
#include "GameInfo.h"

PutDieWithoutNeighborTC::PutDieWithoutNeighborTC()
{
	// ctor
}

ToolCardUseFeedback PutDieWithoutNeighborTC::use(tool_card_move_t m, Game& g)
{
	auto mv     = to_concrete_type<PutDieWithoutNeighborTC::Move>(m);
	auto pBoard = GameInfo::player_on_move(g)->board;

	if (g.get_ctx()->basicMoveHandler->is_reachable(*pBoard, mv->bm.boardIndices)) {
		LOG_D("BadMoveRequest 14");
		throw BadMoveRequest{"A move using this tool card has to be unreachable!"};
	}

	g.move_request(&mv->bm, true);
	base_use_impl();
	return ToolCardUseFeedback{.shiftRound = true, .tcUsageFinished = true, .extractCost = true};
}

void PutDieWithoutNeighborTC::append_possible_moves(ToolCardAppendContext& ctx)
{
	auto& board            = GameInfo::get_player_with_id(ctx.g, ctx.playerId)->board;
	auto& basicMoveHandler = ctx.g.get_ctx()->basicMoveHandler;

	DieToFieldMove dummyDtfm =
	    DieToFieldMove{ctx.playerId, nullptr, BoardIndex{}, DTFM_MODE_ACCEPT_UNREACHABLE};
	for (auto&& die : ctx.g.get_curr_state()->currentRoundRemainingDice) {
		auto indices = basicMoveHandler->indices_to_put_to(ctx.g, ctx.playerId, die,
		                                                   DTFM_MODE_ACCEPT_UNREACHABLE);

		for (auto&& index : indices) {
			if (basicMoveHandler->is_reachable(*board, index)) {
				continue;
			}

			dummyDtfm.boardIndices = index;
			dummyDtfm.die          = die;
			ctx.moveContainer.push_back(
			    new PutDieWithoutNeighborTC::Move(ctx.playerId, ctx.toolCardInd, dummyDtfm));
		}
	}
}

ToolCardUseFeedback PutDieWithoutNeighborTC::undo(tool_card_move_t& m, Game& g)
{
	auto dieWithoutNeighborMove = to_concrete_type<PutDieWithoutNeighborTC::Move>(m);

	g.undo_dtfm(&dieWithoutNeighborMove->bm, true, true);
	base_undo_impl();

	return ToolCardUseFeedback{.shiftRound = false, .tcUsageFinished = true, .extractCost = true};
}

PutDieWithoutNeighborTC::~PutDieWithoutNeighborTC()
{
	// dtor
}
