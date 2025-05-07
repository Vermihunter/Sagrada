#ifndef LOCAL_PLAYER_GAME_TYPE_CHOOSING_SUBPAGE_H
#define LOCAL_PLAYER_GAME_TYPE_CHOOSING_SUBPAGE_H

#include "LocalPlayerGameConfigurationSubpage.h"
#include "Page.h"

class LocalPlayerGameTypeChoosingSubpage : public Page {
  public:
	LocalPlayerGameTypeChoosingSubpage(const ViewContext& ctx);

  private:
	Gtk::Fixed                                           fixed;
	std::unique_ptr<LocalPlayerGameConfigurationSubpage> configChoosingPage;

	Gtk::VBox vbox_GameTypeOptions;

	Gtk::Button button_LocalPlayerVsAi;
	Gtk::Button button_Back;

	void computer_button_clicked();
};

#endif // LOCAL_PLAYER_GAME_TYPE_CHOOSING_SUBPAGE_H