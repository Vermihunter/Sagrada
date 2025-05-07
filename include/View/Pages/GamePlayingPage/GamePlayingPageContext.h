#ifndef GAME_PLAYING_PAGE_CONTEXT_H
#define GAME_PLAYING_PAGE_CONTEXT_H

#include "CardIcon.h"
#include "CommonGameViewElements.h"
#include "ControllerWithAIPlayer.h"
#include "CurrentRoundsDiceView.h"
#include "PageType.h"
#include "PlayerOrderDisplayer.h"
#include "RectangularBoardView.h"
#include "RoundNumberView.h"
#include "RoundTrackView.h"
#include "ScoreBoard.h"
#include "SizeInPixels.h"
#include "ViewContext.h"

#include <gtkmm.h>
#include <memory>
#include <vector>

class GamePlayingPage;

struct GamePlayingPageContext {
	static constexpr size_t wpcPerRow = 2;

	GamePlayingPageContext(const ViewContext& viewCtx, ControllerWithAIPlayer& controller,
	                       std::unique_ptr<CommonGameElements> commonGameElements);

	void construct_scoreboard(eval_state_c&& scores);

	Gtk::Fixed                                         fixed;
	RoundTrackView                                     roundTrackView;
	CurrentRoundsDiceView                              currRoundsDice;
	PlayerOrderDisplayer                               playerOrderDisplayer;
	std::vector<std::unique_ptr<RectangularBoardView>> playerBoards;
	std::unique_ptr<CommonGameElements>                commonGameElements;

	Gtk::Button button_Quit;

	Gtk::VBox vbox_Moves;

	Gtk::Box*   currentlyShownCards;
	card_icon_t puocIcon;
	card_icon_t procIcon;
	card_icon_t tcIcon;
	Gtk::HBox   hbox_icons;
	Gtk::VBox   vbox_iconsWithTooltip;

	RoundNumberView roundNumberView;

	std::unique_ptr<ScoreBoard> scoreBoard;

  private:
	ViewContext ctx;
};

#endif // GAME_PLAYING_PAGE_CONTEXT_H