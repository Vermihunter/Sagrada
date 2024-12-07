#ifndef TOURNAMENT_GAME_RESULT_H
#define TOURNAMENT_GAME_RESULT_H

#include <gtkmm.h>

#include "GameContextBuilder.h"
#include "OnlyAIPlayerController.h"
#include "SimulationGamePlayingPage.h"
#include "TournamentGameInformation.h"
#include "Typedefs.h"
#include "ViewContext.h"

class TournamentPage;

class TournamentGameResult : public Gtk::HBox
{
  public:
    TournamentGameResult(TournamentPage& _parentPage, const ViewContext& _ctx,
                         const ViewContext& _simulationPageCtx,
                         const TournamentGameInformation& _tournamentInfo,
                         size_t _gameCount);

  private:
    TournamentPage& parentPage;
    ViewContext ctx;
    ViewContext simulationPageCtx;
    const TournamentGameInformation& tournamentInfo;

    Gtk::Label label_GameCount;
    Gtk::Label label_UsedSeed;
    Gtk::Label label_Winner;
    Gtk::Button button_SimulateGame;

    std::unique_ptr<SimulationGamePlayingPage> gameSimulatingPage;

    void simulate_button_clicked();
};

#endif // TOURNAMENT_GAME_RESULT_H