#ifndef GAME_PLAYING_PAGE_H
#define GAME_PLAYING_PAGE_H

#include <vector>

#include "DieToFieldMove.h"
#include "GamePlayingPageContext.h"
#include "INewRoundStartSignallable.h"
#include "Move.h"
#include "Page.h"
#include "PassMove.h"
#include "ToolCardView.h"

class GamePlayingPage : public Page, public INewRoundStartSignallable
{
  public:
    GamePlayingPage(const ViewContext& ctx, ControllerWithAIPlayer& _controller)
        : Page(ctx), controller(_controller)
    {
    }

    void hide_non_load_page_widgets() { pageCtx->commonGameElements->hide(); }

    virtual void signal_game_end() override
    {
        pageCtx->currRoundsDice.hide();
        pageCtx->roundNumberView.hide();
        pageCtx->roundTrackView.hide();
        pageCtx->playerOrderDisplayer.hide();
        pageCtx->construct_scoreboard(controller.get_game().evaluate());
    }

    virtual void signal_new_round_start() override
    {
        pageCtx->currRoundsDice.initialize_new_round();
        pageCtx->roundTrackView.queue_draw();
        pageCtx->roundNumberView.queue_draw();
    }

  protected:
    void initialize(GamePlayingPageContext& pageCtx);
    void quit_button_clicked();
    void redraw_all();
    void display_error_message(const std::string& title,
                               const std::string& text);
    void redraw_boards();

  private:
    ControllerWithAIPlayer& controller;
    GamePlayingPageContext* pageCtx;
};

#endif // GAME_PLAYING_PAGE_H
