#include "RerollAllDiceTC.h"

#include "Game.h"

RerollAllDiceTC::RerollAllDiceTC()
{
	// ctor
}

ToolCardUseFeedback RerollAllDiceTC::use(tool_card_move_t m, Game& g)
{
	if (g.get_curr_state()->is_curr_player_first_move_this_round()) {
		LOG_D("BadMoveRequest 26");
		throw BadMoveRequest{"You can use this card only\n    in your second round!"};
	}

	auto mv = to_concrete_type<RerollAllDiceTC::Move>(m);

	for (auto&& die : g.get_curr_state()->currentRoundRemainingDice) {
		mv->originalValues.push_back({die, die->get_value()});

		auto newVal = g.get_ctx()->rnd->roll_die();
		die->change_value(newVal);
	}

	base_use_impl();
	return ToolCardUseFeedback{.shiftRound = false, .tcUsageFinished = true, .extractCost = true};
}

void RerollAllDiceTC::append_possible_moves(ToolCardAppendContext& ctx)
{
	if (!ctx.g.get_curr_state()->is_curr_player_first_move_this_round()) {
		ctx.moveContainer.push_back(new RerollAllDiceTC::Move(ctx.playerId, ctx.toolCardInd));
	}
}

ToolCardUseFeedback RerollAllDiceTC::undo(tool_card_move_t& m, Game& g)
{
	auto mv = to_concrete_type<RerollAllDiceTC::Move>(m);

	for (auto&& [die, origVal] : mv->originalValues) {
		die->change_value(origVal);
	}

	base_undo_impl();

	return ToolCardUseFeedback{.shiftRound = false, .tcUsageFinished = true, .extractCost = true};
}

RerollAllDiceTC::~RerollAllDiceTC()
{
	// dtor
}
