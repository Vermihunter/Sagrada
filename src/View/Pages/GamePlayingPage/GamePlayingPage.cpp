#include "GamePlayingPage.h"

#include "SagradaWindow.h"

void GamePlayingPage::display_error_message(const std::string& title,
                                            const std::string& text)
{
    ctx.mainWindow.display_error_message(title, text);
}

void GamePlayingPage::quit_button_clicked() { ctx.mainWindow.show_home_page(); }

void GamePlayingPage::redraw_all()
{
    pageCtx->roundNumberView.queue_draw();
    pageCtx->roundTrackView.queue_draw();
    pageCtx->currRoundsDice.queue_draw();
    pageCtx->playerOrderDisplayer.queue_draw();
    pageCtx->tcIcon->queue_draw();

    redraw_boards();
}

void GamePlayingPage::redraw_boards()
{
    for (auto&& board : pageCtx->playerBoards) {
        board->queue_draw();
    }
}

void GamePlayingPage::initialize(GamePlayingPageContext& _pageCtx)
{
    pageCtx = &_pageCtx;
}