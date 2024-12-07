#include "ChangeDieValueTC.h"

#include "Game.h"

ChangeDieValueTC::ChangeDieValueTC() {}

/** use() operation begin */

ToolCardUseFeedback ChangeDieValueTC::use(tool_card_move_t m, Game& g)
{
    this_move_t mv = to_concrete_type<ChangeDieValueTC::Move>(m);

    auto& currRoundsDice = g.get_curr_state()->currentRoundRemainingDice;
    if (std::find(currRoundsDice.cbegin(), currRoundsDice.cend(),
                  mv->dtfm.die) == currRoundsDice.cend()) {
        LOG_D("BadMoveRequest 10");
        throw BadMoveRequest {
            "The die has to be one of the current round's dice!"};
    }

    modify_value_move_part(mv, mv->changeVal);
    try {
        g.move_request(&mv->dtfm, true);
    }
    // If the move was not correct, the die has to be set to its previous state
    catch (const BadMoveRequest& bmr) {
        modify_value_move_part(mv, (-1) * mv->changeVal);
        throw bmr;
    }

    base_use_impl();
    return ToolCardUseFeedback {
        .shiftRound = true, .tcUsageFinished = true, .extractCost = true};
}

void ChangeDieValueTC::modify_value_move_part(this_move_t& m, int byVal)
{
    int currVal = int(m->dtfm.die->get_value());
    int newVal = currVal + byVal;

    if (!Value::val_is_correct(newVal)) {
        LOG_D("BadMoveRequest 11");
        throw BadMoveRequest {"Incorrect value change!"};
    }

    m->dtfm.die->change_value(value_t(newVal));
}

/** use() operation end */

/** append_possible_moves() operation begin */

void ChangeDieValueTC::append_possible_moves(ToolCardAppendContext& ctx)
{
    auto& dice = ctx.g.get_curr_state()->currentRoundRemainingDice;
    for (auto&& die : dice) {
        if (die->get_value() != value_t::ONE) {
            process_hypothetical_die_with_val(ctx, die, -1);
        }

        if (die->get_value() != value_t::SIX) {
            process_hypothetical_die_with_val(ctx, die, 1);
        }
    }
}

/**
 * Checks if a die with the hypothetical value (increment/decrement its original
 * value by 1) could be placed on the board somewhere.
 *
 * It first changes the original die's value to the hypothetical value and after
 * checking the possible moves with it, changed back to the original value;
 */
void ChangeDieValueTC::process_hypothetical_die_with_val(
    ToolCardAppendContext& ctx, Die* die, int changeVal)
{
    value_t originalValue = die->get_value();
    value_t hypotheticalValue = value_t(int(originalValue) + changeVal);

    /**
     * Modifies the value of the die so it recreated the future die
     * It cannot create a hypothetical die, because the placing move remembers
     * a pointer to the die that will actually be used in the move.
     */
    die->change_value(hypotheticalValue);

    append_moves_for_die(ctx, die, changeVal);

    /* After the move appendings, the value is changed back to the original
     * value*/
    die->change_value(originalValue);
}

void ChangeDieValueTC::modify_die_value(Die* die, int value)
{
    die->change_value(value_t(int(die->get_value()) + value));
}

void ChangeDieValueTC::append_moves_for_die(ToolCardAppendContext& ctx,
                                            Die* die, int changeVal)
{
    auto possiblePlacingIndices =
        ctx.g.get_ctx()->basicMoveHandler->indices_to_put_to(ctx.g,
                                                             ctx.playerId, die);
    for (auto&& possiblePlacingIndex : possiblePlacingIndices) {
        ctx.moveContainer.push_back(ChangeDieValueTC::Move::from_context(
            ctx, die, changeVal, possiblePlacingIndex));
    }
}

/** append_possible_moves() operation end */

/** undo() operation begin */

ToolCardUseFeedback ChangeDieValueTC::undo(tool_card_move_t& m, Game& g)
{
    this_move_t mv = to_concrete_type<ChangeDieValueTC::Move>(m);

    g.undo_dtfm(&mv->dtfm, true, true);
    // Undone by modifying its value with reverse value
    modify_value_move_part(mv, (-1) * mv->changeVal);
    base_undo_impl();
    return ToolCardUseFeedback {
        .shiftRound = false, .tcUsageFinished = true, .extractCost = true};
}

/** undo() operation end */

ChangeDieValueTC::~ChangeDieValueTC()
{
    // dtor
}

/** Move implementation begin */

tool_card_move_t ChangeDieValueTC::Move::from_context(
    ToolCardAppendContext& ctx, Die* die, int changeVal, const BoardIndex& ind)
{
    return new ChangeDieValueTC::Move(ctx.playerId, ctx.toolCardInd, die,
                                      changeVal, ind);
}

/** Move implementation end */