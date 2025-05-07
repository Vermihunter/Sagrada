#ifndef SIMULATION_GAME_PLAYING_PAGE_h
#define SIMULATION_GAME_PLAYING_PAGE_h

#include "GamePlayingPage.h"
#include "OnlyAIPlayerController.h"
#include "SimulationGamePlayingPageContext.h"

class SimulationGamePlayingPage : public GamePlayingPage {
  public:
	SimulationGamePlayingPage(Page& _prevPage, bool _showSagradaLogoOnQuitButton,
	                          const ViewContext& ctx, only_ai_controller_t _controller,
	                          game_t _game);

	virtual void signal_game_end() override;

  private:
	Page&                            prevPage;
	bool                             showSagradaLogoOnQuitButton;
	SimulationGamePlayingPageContext pageCtx;
	game_t                           game;
	only_ai_controller_t             controller;

	void next_move_button_clicked();
	void quit_button_clicked();
};

#endif // SIMULATION_GAME_PLAYING_PAGE_h