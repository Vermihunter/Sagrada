#include "RerollDieTC.h"

#include "Game.h"

RerollDieTC::RerollDieTC()
{
    // ctor
}

ToolCardUseFeedback RerollDieTC::use(tool_card_move_t m, Game& g)
{
    auto rerollDieMove = dynamic_cast<RerollDieTC::Move*>(m);
    /** First part of the move: rerolling the die */
    if (rerollDieMove) {
        auto newVal = g.get_ctx()->rnd->roll_die();
        rerollDieMove->die->change_value(newVal);

        /**
         * If the die with it's new value could not be placed,
         * the use of the tool card is considered done.
         */
        bool moveExists = move_exists(rerollDieMove, g);
        rerollDieMove->wasPlaceable = moveExists;
        base_use_impl();

        return ToolCardUseFeedback {.shiftRound = !moveExists,
                                    .tcUsageFinished = !moveExists,
                                    .extractCost = true};
    }

    /** Second part of the move: placing the rerolled die */
    auto pdm = to_concrete_type<PlacingDieMove>(m);
    g.move_request(&pdm->bm, true);
    // base_use_impl();

    return ToolCardUseFeedback {
        .shiftRound = true, .tcUsageFinished = true, .extractCost = false};
}

bool RerollDieTC::move_exists(reroll_move_t& rerollSubmove, Game& g)
{
    return g.get_ctx()
               ->basicMoveHandler
               ->indices_to_put_to(g, rerollSubmove->playerId,
                                   rerollSubmove->die)
               .size() > 0;
}

void RerollDieTC::append_possible_moves(ToolCardAppendContext& ctx)
{
    if (!ctx.prevMove) {
        append_reroll_moves(ctx);
        return;
    }

    append_die_placing_moves(ctx);
}

void RerollDieTC::append_reroll_moves(ToolCardAppendContext& ctx)
{
    auto& dice = ctx.g.get_curr_state()->currentRoundRemainingDice;

    for (auto&& die : dice) {
        ctx.moveContainer.push_back(new RerollDieTC::Move(
            ctx.playerId, ctx.toolCardInd, die, die->get_value()));
    }
}

void RerollDieTC::append_die_placing_moves(ToolCardAppendContext& ctx)
{
    auto rerollMove = to_concrete_type<RerollDieTC::Move>(ctx.prevMove);

    append_placing_moves(ctx, rerollMove->die);
}

ToolCardUseFeedback RerollDieTC::undo(tool_card_move_t& m, Game& g)
{
    auto placingMove = dynamic_cast<PlacingDieMove*>(m);
    // This happens first if happens at all because undo happens in reverse
    // order
    if (placingMove) {
        g.undo_dtfm(&placingMove->bm, true, true);

        return ToolCardUseFeedback {.shiftRound = false,
                                    .tcUsageFinished = false,
                                    .extractCost = false};
    }

    reroll_move_t rerollMove = to_concrete_type<RerollDieTC::Move>(m);
    rerollMove->die->change_value(rerollMove->prevVal);
    base_undo_impl();
    return ToolCardUseFeedback {.shiftRound = !rerollMove->wasPlaceable,
                                .tcUsageFinished = true,
                                .extractCost = true};
}

RerollDieTC::~RerollDieTC()
{
    // dtor
}
