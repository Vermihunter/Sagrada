#include "TournamentGameResult.h"

#include "SagradaWindow.h"
#include "TournamentPage.h"

TournamentGameResult::TournamentGameResult(TournamentPage& _parentPage, const ViewContext& _ctx,
                                           const ViewContext&               _simulationPageCtx,
                                           const TournamentGameInformation& _tournamentInfo,
                                           size_t                           _gameCount)
    : parentPage(_parentPage), ctx(_ctx), simulationPageCtx(_simulationPageCtx),
      tournamentInfo(_tournamentInfo), label_GameCount(std::to_string(_gameCount)),
      label_UsedSeed(std::to_string(tournamentInfo.gameInfo->randomSeed)),
      button_SimulateGame("Simulate game")
{
	button_SimulateGame.signal_clicked().connect(
	    sigc::mem_fun(*this, &TournamentGameResult::simulate_button_clicked));

	auto& winnerPlayer   = tournamentInfo.finalScores[0]->get_player();
	auto& winnerAIPlayer = tournamentInfo.controller->get_ai_players().at(winnerPlayer->ID);
	label_Winner.set_text(tournamentInfo.controller->get_player_to_config_mapper()
	                          .at(winnerAIPlayer.get())
	                          ->get_name());

	label_GameCount.set_size_request(ctx.size.width / 4, 10);
	label_UsedSeed.set_size_request(ctx.size.width / 4, 10);
	label_Winner.set_size_request(ctx.size.width / 4, 10);
	button_SimulateGame.set_size_request(ctx.size.width / 4, 10);

	pack_start(label_GameCount);
	pack_start(label_UsedSeed);
	pack_start(label_Winner);
	pack_start(button_SimulateGame);
	show_all_children();
}

void TournamentGameResult::simulate_button_clicked()
{
	auto gameCtx = tournamentInfo.gameInfo->gameCtxBuilder->build(
	    tournamentInfo.gameInfo->players.size(),
	    RandomGenerator::initialize_with_seed(tournamentInfo.gameInfo->randomSeed));
	auto game =
	    std::make_unique<Game>(std::move(gameCtx), DefaultGameCreator::get_default_game_state());

	auto controller = std::make_unique<OnlyAIPlayerController>(
	    *game, tournamentInfo.aiConfigs, tournamentInfo.gameInfo->gameDeterministic);

	gameSimulatingPage = std::make_unique<SimulationGamePlayingPage>(
	    parentPage, false, simulationPageCtx, std::move(controller), std::move(game));

	gameSimulatingPage->hide_non_load_page_widgets();
	ctx.mainWindow.add_page(*gameSimulatingPage);
	ctx.mainWindow.show_page(*gameSimulatingPage);
}