#include "ReverseDieValueTC.h"

#include "Game.h"

ReverseDieValueTC::ReverseDieValueTC()
{
    // ctor
}

ToolCardUseFeedback ReverseDieValueTC::use(tool_card_move_t m, Game& g)
{
    auto mv = to_concrete_type<ReverseDieValueTC::Move>(m);

    reverse_die_value(mv->bm.die);
    try {
        g.move_request(&mv->bm, true);
    }
    catch (BadMoveRequest& bmr) {
        reverse_die_value(mv->bm.die);
        throw bmr;
    }

    base_use_impl();
    return ToolCardUseFeedback {
        .shiftRound = true, .tcUsageFinished = true, .extractCost = true};
}

void ReverseDieValueTC::reverse_die_value(Die* die)
{
    auto oldVal = die->get_value();
    auto newVal = Value::TYPE(size_t(Value::TYPE::COUNT) - size_t(oldVal) - 1);

    die->change_value(newVal);
}

void ReverseDieValueTC::append_possible_moves(ToolCardAppendContext& ctx)
{
    for (auto&& die : ctx.g.get_curr_state()->currentRoundRemainingDice) {
        append_moves_for(die, ctx);
    }
}

void ReverseDieValueTC::append_moves_for(Die* die, ToolCardAppendContext& ctx)
{
    // Reversing the value of the die so the game could check all the possible
    // indices for the hypothetical die that will be the real die when using
    // this TC
    reverse_die_value(die);

    append_placing_moves(ctx, die);

    // Rereversing = making it back to real because the player hasn't used this
    // TC yet
    reverse_die_value(die);
}

ToolCardUseFeedback ReverseDieValueTC::undo(tool_card_move_t& m, Game& g)
{
    auto reverseMove = to_concrete_type<ReverseDieValueTC::Move>(m);

    g.undo_dtfm(&reverseMove->bm, true, true);
    reverse_die_value(reverseMove->bm.die);
    base_undo_impl();

    return ToolCardUseFeedback {
        .shiftRound = false, .tcUsageFinished = true, .extractCost = true};
}

ReverseDieValueTC::~ReverseDieValueTC()
{
    // dtor
}
