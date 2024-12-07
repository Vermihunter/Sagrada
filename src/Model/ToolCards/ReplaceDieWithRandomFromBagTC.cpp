#include "ReplaceDieWithRandomFromBagTC.h"

#include "Game.h"

ReplaceDieWithRandomFromBagTC::ReplaceDieWithRandomFromBagTC() {}

ToolCardUseFeedback ReplaceDieWithRandomFromBagTC::use(tool_card_move_t m,
                                                       Game& g)
{
    /**
     * First part of the move: replacing a die from supply with one from bag
     */
    replace_move_t dieChangingMove =
        dynamic_cast<ReplaceDieWithRandomFromBagTC::DieToChangeMove*>(m);

    if (dieChangingMove) {
        replace_die_move_part(dieChangingMove, g);
        /**
         * Have to call here, because it may happen that the user won't be able
         * to place the die on the board and then the third part of the move is
         * skipped.
         */
        bool moveExists = move_exists(dieChangingMove, g);
        dieChangingMove->wasPlaceable = moveExists;
        base_use_impl();

        return ToolCardUseFeedback {.shiftRound = !moveExists,
                                    .tcUsageFinished = !moveExists,
                                    .extractCost = true};
        //   return ToolCardUseFeedback{.shiftRound = true, .tcUsageFinished =
        //   true, .extractCost = true};
    }

    /**
     * Second part of the move: choosing value for the replaced die and placing
     * it to a field
     */
    placing_move_t valueChangingMove =
        to_concrete_type<ValueChangerAndPlacerMove>(m);

    valueChangingMove->placingMove.die->change_value(valueChangingMove->newVal);
    g.move_request(&valueChangingMove->placingMove, true);

    return ToolCardUseFeedback {
        .shiftRound = true, .tcUsageFinished = true, .extractCost = false};
}

void ReplaceDieWithRandomFromBagTC::replace_die_move_part(replace_move_t m,
                                                          Game& g)
{
    auto& diceSupply = g.get_ctx()->supply;

    Die* unusedDie =
        diceSupply->get_unused_die(g.get_curr_state()->roundNumber);

    if (unusedDie == nullptr) {
        LOG_D("BadMoveRequest 25");
        throw BadMoveRequest {"No more dice in the bag!"};
    }

    m->dieInBag = unusedDie;
    std::swap(*m->dieInBag, *m->currentlyUsedDie);
}

bool ReplaceDieWithRandomFromBagTC::move_exists(replace_move_t& replacingMove,
                                                Game& g)
{
    Die* die = replacingMove->currentlyUsedDie;
    value_t dieOriginalValue = die->get_value();

    for (size_t i = 0; i < size_t(value_t::COUNT); ++i) {
        value_t _val = value_t(i);
        die->change_value(_val);

        if (g.get_ctx()
                ->basicMoveHandler
                ->indices_to_put_to(g, replacingMove->playerId, die)
                .size() > 0) {
            die->change_value(dieOriginalValue);
            return true;
        }
    }

    die->change_value(dieOriginalValue);
    return false;
}

// Same base with RerollDieTC because the second step (putting the selected die)
// is the same

void ReplaceDieWithRandomFromBagTC::append_possible_moves(
    ToolCardAppendContext& ctx)
{
    if (!ctx.prevMove) {
        append_die_changing_moves(ctx);
        return;
    }

    append_die_placing_moves(ctx);
}

void ReplaceDieWithRandomFromBagTC::append_die_changing_moves(
    ToolCardAppendContext& ctx)
{
    /** There are no dice in the bag to swap the ones from the supply with */
    if (ctx.g.get_ctx()->supply->empty(ctx.g.get_curr_state()->roundNumber)) {
        return;
    }

    /** Any die could be replaced from the supply for one from the bag */
    for (auto&& die : ctx.g.get_curr_state()->currentRoundRemainingDice) {
        ctx.moveContainer.push_back(
            new ReplaceDieWithRandomFromBagTC::DieToChangeMove(
                ctx.playerId, ctx.toolCardInd, die));
    }
}

void ReplaceDieWithRandomFromBagTC::append_die_placing_moves(
    ToolCardAppendContext& ctx)
{
    replace_move_t prevDieChangingMove =
        to_concrete_type<DieToChangeMove>(ctx.prevMove);

    Die* currentlyUsedDie = prevDieChangingMove->currentlyUsedDie;

    value_t originalValue = currentlyUsedDie->get_value();

    // try out every value
    for (size_t i = 0; i < size_t(value_t::COUNT); ++i) {
        value_t newVal = value_t(i);
        currentlyUsedDie->change_value(newVal);
        auto indicesToPutTo =
            ctx.g.get_ctx()->basicMoveHandler->indices_to_put_to(
                ctx.g, ctx.playerId, prevDieChangingMove->currentlyUsedDie);
        for (auto&& index : indicesToPutTo) {
            ctx.moveContainer.push_back(new ValueChangerAndPlacerMove(
                ctx.playerId, ctx.toolCardInd, newVal, originalValue,
                currentlyUsedDie, index));
        }
    }

    currentlyUsedDie->change_value(originalValue);
}

ToolCardUseFeedback ReplaceDieWithRandomFromBagTC::undo(tool_card_move_t& m,
                                                        Game& g)
{
    auto valueChangerMove =
        dynamic_cast<ReplaceDieWithRandomFromBagTC::ValueChangerAndPlacerMove*>(
            m);

    // This happens first, if the die from the bag was placeable,
    // if it wasn't, this submove didn't happen -> nothing to undo
    if (valueChangerMove) {
        valueChangerMove->placingMove.die->change_value(
            valueChangerMove->prevVal);
        g.undo_dtfm(&valueChangerMove->placingMove, true, true);

        return ToolCardUseFeedback {.shiftRound = false,
                                    .tcUsageFinished = false,
                                    .extractCost = false};
    }

    // This happens last -> ToolCard::undo() called
    auto dieSwitchingMove = to_concrete_type<DieToChangeMove>(m);
    std::swap(*dieSwitchingMove->currentlyUsedDie, *dieSwitchingMove->dieInBag);
    dieSwitchingMove->dieInBag = nullptr;
    base_undo_impl();
    return ToolCardUseFeedback {.shiftRound = !dieSwitchingMove->wasPlaceable,
                                .tcUsageFinished = true,
                                .extractCost = true};
    //  return ToolCardUseFeedback{.shiftRound = true, .tcUsageFinished = true,
    //  .extractCost = true};
}

ReplaceDieWithRandomFromBagTC::~ReplaceDieWithRandomFromBagTC()
{
    // dtor
}
