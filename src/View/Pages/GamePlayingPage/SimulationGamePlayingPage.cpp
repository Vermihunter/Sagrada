#include "SimulationGamePlayingPage.h"

#include "SagradaWindow.h"

SimulationGamePlayingPage::SimulationGamePlayingPage(
    Page& _prevPage, bool _showSagradaLogoOnQuitButton, const ViewContext& ctx,
    only_ai_controller_t _controller, game_t _game)
    : GamePlayingPage(ctx, *_controller)
    , prevPage(_prevPage)
    , showSagradaLogoOnQuitButton(_showSagradaLogoOnQuitButton)
    , pageCtx(ctx, *_controller)
    , game(std::move(_game))
    , controller(std::move(_controller))
{
    initialize(pageCtx);

    pageCtx.button_Next.signal_clicked().connect(sigc::mem_fun(
        *this, &SimulationGamePlayingPage::next_move_button_clicked));

    pageCtx.button_Quit.signal_clicked().connect(
        sigc::mem_fun(*this, &SimulationGamePlayingPage::quit_button_clicked));

    add(pageCtx.fixed);
    show_all_children();
}

void SimulationGamePlayingPage::signal_game_end()
{
    GamePlayingPage::signal_game_end();
    pageCtx.button_Next.hide();
}

void SimulationGamePlayingPage::next_move_button_clicked()
{
    size_t prevRoundNumber =
        controller->get_game().get_curr_state()->roundNumber;
    controller->make_next_ai_player_move();
    redraw_all();

    if (controller->get_game().get_curr_state()->hasEnded) {
        signal_game_end();
    }
    else if (prevRoundNumber !=
             controller->get_game().get_curr_state()->roundNumber) {
        signal_new_round_start();
    }
}

void SimulationGamePlayingPage::quit_button_clicked()
{
    ctx.mainWindow.show_page(prevPage, showSagradaLogoOnQuitButton);
}