#include "LocalGamePlayingPageContext.h"

#include "LocalGamePlayingPage.h"

LocalGamePlayingPageContext::LocalGamePlayingPageContext(
    LocalGamePlayingPage& parent, LocalPlayerController& controller,
    const ViewContext& viewCtx,
    std::unique_ptr<CommonGameElements> _commonGameElements)
    : GamePlayingPageContext(viewCtx, controller,
                             std::move(_commonGameElements))
    , button_PassMove("Pass")
{
    vbox_Moves.pack_start(button_PassMove);

    for (auto&& tc_view : commonGameElements->tcViews.tcCards) {
        tc_view->set_signal_receiver(&parent);
    }

    for (auto&& playerWpc : playerBoards) {
        playerWpc->subscribe(&parent);
    }

    roundTrackView.subscribe(&parent);
    currRoundsDice.subscribe(&parent);
}