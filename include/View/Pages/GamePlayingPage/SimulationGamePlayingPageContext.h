#ifndef SIMULATION_GAME_PLAYING_PAGE_CONTEXT_H
#define SIMULATION_GAME_PLAYING_PAGE_CONTEXT_H

#include "CommonGameViewElements.h"
#include "GamePlayingPageContext.h"

struct SimulationGamePlayingPageContext : public GamePlayingPageContext
{
    SimulationGamePlayingPageContext(const ViewContext& viewCtx,
                                     ControllerWithAIPlayer& controller)
        : GamePlayingPageContext(
              viewCtx, controller,
              CommonGameElements::create(controller.get_game(),
                                         controller.get_all_ids(), 155))
        , button_Next("Next")
    {
        vbox_Moves.pack_start(button_Next);
    }

    Gtk::Button button_Next;
};

#endif // SIMULATION_GAME_PLAYING_PAGE_CONTEXT_H