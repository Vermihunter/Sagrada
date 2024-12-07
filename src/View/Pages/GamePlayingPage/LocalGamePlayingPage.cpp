#include "LocalGamePlayingPage.h"

#include "SagradaWindow.h"

LocalGamePlayingPage::LocalGamePlayingPage(
    const ViewContext& ctx, local_player_controller_t _controller,
    std::unique_ptr<CommonGameElements> commonGameElements, game_t _game)
    : GamePlayingPage(ctx, *_controller)
    , pageCtx(*this, *_controller, ctx, std::move(commonGameElements))
    , currState(pageCtx, defaultState)
    , controller(std::move(_controller))
    , game(std::move(_game))
{
    initialize(pageCtx);

    pageCtx.button_PassMove.signal_clicked().connect(
        sigc::mem_fun(*this, &LocalGamePlayingPage::pass_move_button_clicked));

    pageCtx.button_Quit.signal_clicked().connect(
        sigc::mem_fun(*this, &LocalGamePlayingPage::quit_button_clicked));

    if (!controller->local_player_on_move()) {
        set_timeout();
    }

    add(pageCtx.fixed);
    show_all_children();
}

void LocalGamePlayingPage::receive_round_track_die_click_signal(Die* die)
{
    std::cout << "LocalGamePlayingPage::receive_round_track_die_click_signal()"
              << std::endl;
    if (currState.blockState.blockedRoundTrackClick) {
        return;
    }

    if (currState.selectedTc) {
        std::cout << "LocalGamePlayingPage::receive_round_track_die_click_"
                     "signal() - signal sent"
                  << std::endl;
        currState.selectedTc->receive_round_track_die_click_signal(die);
    }
}

void LocalGamePlayingPage::receive_die_click_signal(Die* die)
{
    if (currState.blockState.blockedDieClick) {
        return;
    }

    if (!controller->local_player_on_move()) {
        display_error_message("Invalid click",
                              " You can select a die only in your turn!");
        return;
    }

    currState.select_die(die);
    deselect_selected_fields();

    if (currState.selectedTc) {
        currState.selectedTc->receive_die_click_signal(die);
    }
    else {
        highlight_basic_moves_for_die(
            die, game->get_curr_state()->player_on_move_id());
    }
}

void LocalGamePlayingPage::receive_wpc_click_signal(RectangularBoardView*) {}

void LocalGamePlayingPage::receive_board_field_click_signal(
    RectangularBoardView* board, const BoardIndex& ind)
{
    if (currState.blockState.blockedBoardClick) {
        return;
    }

    // There is a selected tool card and has to be signalled for the events
    if (currState.selectedTc) {
        return currState.selectedTc->receive_board_field_click_signal(board,
                                                                      ind);
    }

    Die* selectedDie = currState.selectedDie;
    if (!selectedDie) {
        return display_error_message("No selected die!",
                                     "Select a die to place");
    }

    currState.unselect_selected_die();

    const bool basicMoveSucceeded = send_move_request(
        DieToFieldMove::create(0, selectedDie, ind), dtfmResultHandlingInfo);

    if (basicMoveSucceeded) {
        deselect_selected_fields();
        pageCtx.currRoundsDice.queue_draw();
        board->queue_draw();
    }
    else {
        board->unselect_all_fields();
        board->queue_draw();
    }
}

void LocalGamePlayingPage::block_features(
    const BlockedUserInterraction& newState)
{
    currState.blockState = newState;
}

void LocalGamePlayingPage::restore_default_blocking_state()
{
    block_features(defaultState);
}

void LocalGamePlayingPage::signal_new_round_start()
{
    GamePlayingPage::signal_new_round_start();
    currState.unselect_tc();
}

void LocalGamePlayingPage::signal_game_end()
{
    GamePlayingPage::signal_game_end();
    pageCtx.button_PassMove.hide();
}

void LocalGamePlayingPage::redraw_curr_rounds_dice()
{
    // pageCtx.currRoundsDice.queue_draw();
    redraw_all();
}

void LocalGamePlayingPage::tool_card_clicked(ToolCardView* clickedTcView,
                                             bool forceSelect)
{
    if (!forceSelect && currState.blockState.blockedTcClick) {
        return;
    }

    ID_t playerOnMoveID =
        controller->get_game().get_curr_state()->player_on_move_id();
    if (!forceSelect && !controller->is_local_player_id(playerOnMoveID)) {
        ctx.mainWindow.display_error_message(
            "It's not your turn!", "You can use tool cards only in your turn!");
        return;
    }

    currState.select_tc(clickedTcView);
    deselect_selected_fields();
    currState.unselect_selected_die();
}

bool LocalGamePlayingPage::send_move_request(
    move_t m, const MoveResultHandlingPolicy& resultHandlingInfo)
{
    bool moveWasCorrect = true;

    Die* selectedDie = currState.selectedDie;
    currState.unselect_selected_die();
    GameState& gameState = *controller->get_game().get_curr_state();
    size_t prevRoundNumber = gameState.roundNumber;

    try {
        controller->make_local_player_move_request(m);
    }
    catch (const BadMoveRequest& bmr) {
        display_error_message("Invalid move!", bmr.what());
        moveWasCorrect = false;

        if (selectedDie) {
            currState.select_die(selectedDie);
        }
        // In case an error occurs, the move is not deleted
        delete m;
    }

    if ((!moveWasCorrect && resultHandlingInfo.deselectOnFailure) ||
        (moveWasCorrect && resultHandlingInfo.deselectOnSuccess)) {
        currState.unselect_tc();
        currState.unselect_selected_die();
        deselect_selected_fields();
    }

    if (!gameState.hasEnded && prevRoundNumber != gameState.roundNumber) {
        signal_new_round_start();
    }

    if (gameState.hasEnded) {
        signal_game_end();
    }
    else if (!controller->local_player_on_move()) {
        set_timeout();
    }

    redraw_all();

    return moveWasCorrect;
}

void LocalGamePlayingPage::show_error_message(const std::string& msg)
{
    ctx.mainWindow.display_error_message("Invalid request!", msg);
}

void LocalGamePlayingPage::highlight_basic_moves_for_die(Die* die,
                                                         ID_t playerID,
                                                         dtfm_mode_mask_t mode)
{
    highlight_fields(game->get_ctx()->basicMoveHandler->indices_to_put_to(
                         *game, playerID, die, mode),
                     playerID);
}

void LocalGamePlayingPage::highlight_fields(const board_ind_c& indices,
                                            ID_t playerID)
{
    RectangularBoardView& board = board_by_player_id(playerID);
    board.unselect_all_fields();
    board.select_fields(indices, &orange);
}

void LocalGamePlayingPage::quit_move_button_clicked()
{
    ctx.mainWindow.show_home_page();
}

void LocalGamePlayingPage::pass_move_button_clicked()
{
    if (!controller->local_player_on_move()) {
        return display_error_message("Invalid move!", "Its not your turn!");
    }

    if (currState.blockState.blockedPassMove) {
        return display_error_message("Invalid move!", "Pass move is blocked!");
    }

    send_move_request(
        PassMove::create(
            controller->get_game().get_curr_state()->player_on_move_id()),
        passMoveResultHandlingInfo);
}

void LocalGamePlayingPage::set_timeout()
{
    auto timeOutInMs = 450;
    timeoutHandler = Glib::signal_timeout().connect(
        sigc::mem_fun(*this, &LocalGamePlayingPage::on_timeout), timeOutInMs);
}

bool LocalGamePlayingPage::on_timeout()
{
    if (controller->local_player_on_move()) {
        return false;
    }

    auto& player = GameInfo::player_on_move(*game);
    size_t prevRoundNumber =
        controller->get_game().get_curr_state()->roundNumber;
    controller->make_next_ai_player_move();
    redraw_all();

    EvalState::for_player(player, *game);

    if (controller->get_game().get_curr_state()->hasEnded) {
        signal_game_end();
        return false;
    }

    if (prevRoundNumber !=
        controller->get_game().get_curr_state()->roundNumber) {
        signal_new_round_start();
    }

    return !controller->local_player_on_move();
}

void LocalGamePlayingPage::deselect_selected_fields()
{
    for (auto&& localPlayerID : controller->get_local_player_ids()) {
        board_by_player_id(localPlayerID).unselect_all_fields();
    }
}

RectangularBoardView& LocalGamePlayingPage::board_by_player_id(ID_t playerID)
{
    return **std::find_if(
        pageCtx.playerBoards.begin(), pageCtx.playerBoards.end(),
        [&](const std::unique_ptr<RectangularBoardView>& board) {
            return board->get_player_info()->ID == playerID;
        });
}
