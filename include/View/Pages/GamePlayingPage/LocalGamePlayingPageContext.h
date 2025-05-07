#ifndef LOCAL_GAME_PLAYING_PAGE_CONTEXT_H
#define LOCAL_GAME_PLAYING_PAGE_CONTEXT_H

#include "GamePlayingPageContext.h"
#include "LocalPlayerController.h"

class LocalGamePlayingPage;

struct LocalGamePlayingPageContext : public GamePlayingPageContext {
  public:
	LocalGamePlayingPageContext(LocalGamePlayingPage& _parent, LocalPlayerController& controller,
	                            const ViewContext&                  viewCtx,
	                            std::unique_ptr<CommonGameElements> commonGameElements);

	Gtk::Button button_PassMove;
};

#endif // LOCAL_GAME_PLAYING_PAGE_CONTEXT_H