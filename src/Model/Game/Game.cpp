#include "Game.h"

#include "Deallocator.h"
#include "EvalState.h"
#include "PassMove.h"
#include "PutDieWithoutNeighborTC.h"
#include "RelocateMaxTwoSameColorDiceTC.h"

Game::Game(game_context_t _ctx, game_state_t _state)
    : ctx(std::move(_ctx)), currState(std::move(_state))
{
    fastctx = ctx.get();
    plan_next_round();
    history.roundsHistory.pop_back();
}

// Cloning with equal but not equivalent information
Game::Game(const Game& g, rnd_t rnd)
    : ctx(g.fastctx->clone(std::move(rnd)))
    , fastctx(ctx.get())
    , currState(g.currState->clone(*g.fastctx->supply, *fastctx->supply))
    , history(g.history, *g.get_ctx()->supply, *fastctx->supply)
{
}

// Identical cloning
Game::Game(const Game& g)
    : ctx(g.fastctx->clone(g.fastctx->rnd->clone()))
    , fastctx(ctx.get())
    , currState(g.currState->clone_identical())
    , history(g.history)
{
}

std::string dice_to_str(const std::vector<Die*>& dice)
{
    std::ostringstream diceStr;
    diceStr << "Current round dice: [" << dice[0];
    for (size_t i = 1; i < dice.size(); ++i) {
        diceStr << ", " << dice[i];
    }
    diceStr << "]";
    return diceStr.str();
}

game_t Game::clone_with_pseudo_random_future(ID_t focusedPlayerID, int seed)
{
    auto gameClone = clone_with_seed(seed);
    // Randomizing future information -> dice
    gameClone->fastctx->supply->randomize_future_dice(currState->roundNumber);

    // Randomizing the Private objective card colors for every player except for
    // the player having ID == focusedPlayerID
    std::vector<color_t> allColors = Color::all_colors();
    gameClone->fastctx->rnd->shuffle(allColors);

    size_t colorInd = 0;
    const player_t& focusedPlayer =
        GameInfo::get_player_with_id(*gameClone, focusedPlayerID);
    for (auto&& player : gameClone->fastctx->players) {
        if (player->ID == focusedPlayerID) {
            continue;
        }

        if (allColors[colorInd] == focusedPlayer->PrOC->get_color()) {
            ++colorInd;
        }

        player->PrOC =
            std::make_shared<PrivateObjectiveCard>(allColors[colorInd]);
        ++colorInd;
    }

    return gameClone;
}

void Game::move_request(move_t m, bool allowDieToFieldMove)
{
    if (currState->hasEnded) {
        LOG_D("BadMoveRequest 4");
        throw BadMoveRequest {"Game has already ended!"};
    }

    if (m->playerId != currState->player_on_move_id()) {
        LOG_D("BadMoveRequest 5");
        throw BadMoveRequest {"You can make moves only in your turns!"};
    }

    switch (m->t) {
        case Move::type::DIE_TO_FIELD_MOVE:
            basic_move_request(DieToFieldMove::cast(m), allowDieToFieldMove);
            break;
        case Move::type::TOOL_CARD:
            tool_card_move_request(m);
            break;
        case Move::type::PASS_MOVE:
            plan_next_player_on_move();
            history.moveStack.push_back(MoveInfo {m, nullptr});
            break;
        default:
            throw std::invalid_argument {"Undefined move type!"};
    }
}

void Game::basic_move_request(die_to_field_move_t mv, bool isToolCardSubmove)
{
    if (currState->previousToolCardMove && !isToolCardSubmove) {
        LOG_D("BadMoveRequest 6");
        throw BadMoveRequest {
            "Currently the selected tool card has to be used!"};
    }

    /** Throws BadMoveRequest exception if the move is incorrect */
    fastctx->basicMoveHandler->check_move_correct_hardfail(*this, mv,
                                                           isToolCardSubmove);
    fastctx->basicMoveHandler->use(*this, mv);

    if (auto dtfm = dynamic_cast<DieToFieldMove*>(mv)) {
        remove_die_from_currents(dtfm->die, !isToolCardSubmove);
    }

    if (!isToolCardSubmove) {
        plan_next_player_on_move();
        history.moveStack.push_back(MoveInfo {mv, nullptr});
        // Otherwise, it is deleted by the ToolCardMove
    }
}

void Game::remove_die_from_currents(Die* die, bool throwOnNotFound)
{
    auto diceIt = std::find(currState->currentRoundRemainingDice.begin(),
                            currState->currentRoundRemainingDice.end(), die);

    bool diceFound = (diceIt != currState->currentRoundRemainingDice.end());

    if (throwOnNotFound && !diceFound) {
        throw std::runtime_error {
            "Die not found between the current round's dice!"};
    }

    if (diceFound) {
        currState->currentRoundRemainingDice.erase(diceIt);
        currState->currRoundInfo->turnInfos[currState->currPlayerIndex]
            .dieOriginalInd =
            std::distance(currState->currentRoundRemainingDice.begin(), diceIt);
    }
}

void Game::tool_card_move_request(move_t m)
{
    if (currState->currRoundInfo->turnInfos[currState->currPlayerIndex]
            .tcUsed &&
        !currState->previousToolCardMove) {
        LOG_D("BadMoveRequest 7");
        throw BadMoveRequest {"You can use at most one Tool card in a round!"};
    }

    auto tc_m = dynamic_cast<ToolCardMove*>(m);
    if (!tc_m) {
        LOG_D("BadMoveRequest 8");
        throw BadMoveRequest {"Not a tool card move!"};
    }

    size_t& favorTokensLeft =
        GameInfo::get_player_with_id(*this, m->playerId)->favorTokensLeft;
    size_t favorTokensToUse =
        fastctx->toolCards[tc_m->ind]->favor_tokens_to_use();

    /**
     * Remembering the cost of using the tool card at its first sub-move
     */
    if (!currState->previousToolCardMove) {
        if (favorTokensLeft < favorTokensToUse) {
            throw BadMoveRequest {
                "You don't have enough favor tokens to use this tool card!"};
        }
    }

    /** The return value indicates whether it was the last move of the tool card
     * or not */
    ToolCardUseFeedback tcFeedback =
        fastctx->toolCards[tc_m->ind]->use(tc_m, *this);
    if (tcFeedback.extractCost) {
        favorTokensLeft -= favorTokensToUse;
    }

    history.moveStack.push_back(
        MoveInfo {tc_m, currState->previousToolCardMove});
    if (tcFeedback.tcUsageFinished) {
        currState->previousToolCardMove = nullptr;
        currState->currRoundInfo->turnInfos[currState->currPlayerIndex].tcUsed =
            true;
    }
    else {
        /** Saving it only if there are submoves left for this tool card move */
        currState->previousToolCardMove = tc_m;
    }

    if (tcFeedback.shiftRound) {
        plan_next_player_on_move();
    }
}

void Game::plan_next_player_on_move()
{
    ++currState->currPlayerIndex;

    if (currState->currPlayerIndex ==
        currState->currRoundInfo->turnInfos.size()) {
        put_remaining_dice_to_round_track();
        currState->currPlayerIndex = 0;
        plan_next_round();
        if (currState->roundNumber == fastctx->numberOfRounds + 1) {
            currState->hasEnded = true;
            return;
        }
    }
}

void Game::plan_next_round()
{
    currState->currentRoundRemainingDice =
        fastctx->supply->get_current_round_dice(currState->roundNumber);
    // LOG_D(dice_to_str(currState->currentRoundRemainingDice));

    ++currState->roundNumber;

    /** Shift the starting player to the right by one */
    currState->playerStartedThisRoundIndex =
        (currState->playerStartedThisRoundIndex + 1) % ctx->players.size();

    history.roundsHistory.push_back(std::move(currState->currRoundInfo));
    currState->currRoundInfo = std::make_unique<RoundInfo>();
    plan_player_order();
}

void Game::plan_player_order()
{
    size_t playerCount = fastctx->players.size();

    /** First in order */
    for (size_t i = 0; i < playerCount; ++i) {
        ID_t nextId =
            fastctx
                ->players[(i + currState->playerStartedThisRoundIndex) %
                          playerCount]
                ->ID;

        /** Adding information that in the next round non of the players has
         * made a moves yet */
        currState->currRoundInfo->turnInfos.push_back(TurnInfo {nextId, false});
    }

    /** After that in reverse order */
    for (size_t i = 0; i < playerCount; ++i) {
        currState->currRoundInfo->turnInfos.push_back(
            currState->currRoundInfo->turnInfos[playerCount - i - 1]);
    }
}

void print_all_moves(move_c& moves)
{
    for (auto&& move : moves) {
        LOG_D(move);
    }
}

move_t Game::get_single_move(single_move_choice_strategy_t strategy)
{
    switch (strategy) {
        case SINGLE_MOVE_STRATEGY_FIRST:
            return get_first_possible_move();
        case SINGLE_MOVE_STRATEGY_RANDOM:
            return get_random_possible_move();
        default:
            return nullptr;
    }
}

move_t Game::get_first_possible_move()
{
    const player_t& player = GameInfo::player_on_move(*this);
    move_c possibleMoves;
    ToolCardAppendContext toolCardCtx {possibleMoves, *this, 0, player->ID,
                                       currState->previousToolCardMove};

    if (currState->previousToolCardMove) {
        auto tcInd = currState->previousToolCardMove->ind;
        toolCardCtx.toolCardInd = tcInd;
        fastctx->toolCards[tcInd]->append_possible_moves(toolCardCtx);
        free_all(possibleMoves.begin() + 1, possibleMoves.end());
        return possibleMoves[0];
    }

    if (!currState->currRoundInfo->turnInfos[currState->currPlayerIndex]
             .tcUsed) {
        toolCardCtx.toolCardInd = 0;

        for (auto&& tc : fastctx->toolCards) {
            if (player->favorTokensLeft >= tc->favor_tokens_to_use()) {
                tc->append_possible_moves(toolCardCtx);
            }

            if (!possibleMoves.empty()) {
                free_all(possibleMoves.begin() + 1, possibleMoves.end());
                return possibleMoves[0];
            }

            ++toolCardCtx.toolCardInd;
        }
    }

    move_t firstBasicMove =
        fastctx->basicMoveHandler->get_first_basic_move(*this, player->ID);
    if (firstBasicMove != nullptr) {
        return firstBasicMove;
    }

    return PassMove::create(player->ID);
}

move_t Game::get_random_possible_move() { return nullptr; }

move_c Game::possible_moves()
{
    if (currState->hasEnded) {
        throw std::runtime_error {"The game has already ended!"};
    }

    const player_t& player = GameInfo::player_on_move(*this);

    move_c possibleMoves;
    ToolCardAppendContext toolCardCtx {possibleMoves, *this, 0, player->ID,
                                       currState->previousToolCardMove};

    /**
     * Previously a tool card has been used, appended their moves for it and
     * at least one more continuation exists, so the user has to choose from
     * them.
     */
    if (currState->previousToolCardMove) {
        auto tcInd = currState->previousToolCardMove->ind;
        toolCardCtx.toolCardInd = tcInd;
        fastctx->toolCards[tcInd]->append_possible_moves(toolCardCtx);
        return possibleMoves;
    }

    /**
     * The player has not used a tool card previously this round,
     * so appending all the accessible tool card moves.
     */
    if (!currState->currRoundInfo->turnInfos[currState->currPlayerIndex]
             .tcUsed) {
        toolCardCtx.toolCardInd = 0;

        for (auto&& tc : fastctx->toolCards) {
            if (player->favorTokensLeft >= tc->favor_tokens_to_use()) {
                tc->append_possible_moves(toolCardCtx);
            }

            ++toolCardCtx.toolCardInd;
        }
    }

    /** basic moves */
    fastctx->basicMoveHandler->append_basic_moves(possibleMoves, *this,
                                                  player->ID);
    possibleMoves.push_back(PassMove::create(player->ID));

    return possibleMoves;
}

void Game::set_board(ID_t playerID, board_t board)
{
    auto& player = GameInfo::get_player_with_id(*this, playerID);

    player->board = board;
    player->favorTokensLeft = board->get_ctx()->hardness;
}

void Game::put_remaining_dice_to_round_track()
{
    fastctx->roundTrack.put_dice(currState->currentRoundRemainingDice);
}

/**
 * It doesn´t allow to use undo operation on any of the
 * non-last moves for several reasons e.g.
 * - whose turn it is after undoing a move of that kind?
 *
 */
void Game::undo_last_move()
{
    if (history.moveStack.empty()) {
        LOG_E("Undo called when there were no moves made yet!");
        return;
    }

    MoveInfo lastMove = history.moveStack.back();
    history.moveStack.pop_back();

    switch (lastMove.move->t) {
        case Move::type::DIE_TO_FIELD_MOVE:
            undo_dtfm((die_to_field_move_t)(lastMove.move), true, true);
            break;
        case Move::type::TOOL_CARD:
            undo_tool_card_move(lastMove);
            break;
        case Move::type::PASS_MOVE:
            shift_order_backwards();
            break;
        default:
            throw std::invalid_argument {"Undefined move type to Undo!"};
    }

    /*
    delete lastMove.move;
    if(lastMove.previousToolCardMove) {
        delete lastMove.previousToolCardMove;
    }
    */
    // delete lastMove;
}

void Game::undo_dtfm(die_to_field_move_t m, bool putDieBackToCurrRoundsDice,
                     bool shiftRoundsBack)
{
    Die* removedDie = fastctx->basicMoveHandler->undo(*this, m);

    if (shiftRoundsBack) {
        shift_order_backwards();
    }

    // In case it was a Pass move, the basic move handler doesn't return a die
    if (putDieBackToCurrRoundsDice && removedDie) {
        currState->currentRoundRemainingDice.insert(
            currState->currentRoundRemainingDice.begin() +
                currState->currRoundInfo->turnInfos[currState->currPlayerIndex]
                    .dieOriginalInd,
            removedDie);
    }
}

/**
 * @brief
 * Re-adding the favor tokens to the player for the undone tool card use
 * is part of the ToolCard::use() function because the different tool cards
 * has to readd in different situations.
 *
 * @param m
 */
void Game::undo_tool_card_move(MoveInfo& m)
{
    tool_card_move_t tcm = dynamic_cast<ToolCardMove*>(m.move);
    if (!tcm) {
        LOG_D("BadMoveRequest 9");
        throw BadMoveRequest {"Invalid move definition. Should be Tool card "
                              "move, but cannot cast!"};
    }

    ToolCardUseFeedback tcUndoFeedback =
        fastctx->toolCards[tcm->ind]->undo(tcm, *this);

    /**
     * Reseting used move for the player
     * This call resets the currentRoundRemainingDice but what if the Tool card
     * undo puts back a die?
     */
    if (tcUndoFeedback.shiftRound) {
        shift_order_backwards();
    }

    // Is this always correct? - yes because it must have been false before
    // using the TC, otherwise the player would not have be able to use it, for
    // multi submove tc moves it is ignored???
    if (tcUndoFeedback.tcUsageFinished) {
        currState->currRoundInfo->turnInfos[currState->currPlayerIndex].tcUsed =
            false;
    }

    if (tcUndoFeedback.extractCost) {
        fastctx->players[tcm->playerId]->favorTokensLeft +=
            fastctx->toolCards[tcm->ind]->favor_tokens_to_use();
    }

    // The flag tcUsageFinished is true if the currently undone tool card move
    // was the first submove There is one element in turnInfos for 2 submoves
    // for ReplaceDieWithRandomFromBagTC / RerollDieTC So sending the second
    // move makes the .previousToolCardMove nullptr and nothing to restore ->
    // store it on the stack
    currState->previousToolCardMove = m.previousToolCardMove;
}

void Game::shift_order_backwards()
{
    if (currState->currPlayerIndex > 0) {
        --currState->currPlayerIndex;
        // currState->previousToolCardMove =
        // currState->currRoundInfo->turnInfos[currState->currPlayerIndex].previousToolCardMove;
        return;
    }

    if (currState->roundNumber == 0) {
        LOG_E(
            "Shift order backwards called when there were no moves made yet!");
        return;
    }

    --currState->roundNumber;
    currState->playerStartedThisRoundIndex =
        (currState->playerStartedThisRoundIndex > 0)
            ? currState->playerStartedThisRoundIndex - 1
            : fastctx->players.size() - 1;

    // Last player of the round
    currState->currPlayerIndex = 2 * ctx->players.size() - 1;

    // Not plan player order but revive from history
    currState->currRoundInfo = std::move(history.roundsHistory.back());
    history.roundsHistory.pop_back();

    //    currState->previousToolCardMove =
    //    currState->currRoundInfo->turnInfos[currState->currPlayerIndex].previousToolCardMove;
    currState->currentRoundRemainingDice = ctx->roundTrack.undo();
    currState->hasEnded = false;
}

eval_state_c Game::evaluate() { return EvalState::for_players(*this); }
