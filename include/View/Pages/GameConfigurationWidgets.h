#ifndef GAME_CONFIGURATION_WIDGETS_H
#define GAME_CONFIGURATION_WIDGETS_H

#include "DefaultGameCreator.h"
#include "GameCache.h"
#include "ViewContext.h"

#include <gtkmm.h>

class HomePageSubPage;

class GameConfigurationWidgets : public Gtk::VBox {
  public:
	GameConfigurationWidgets(const ViewContext&                  _ctx,
	                         game_ctx_builder_by_player_count_t& allCtxBuilders,
	                         size_t                              _localPlayerCount);

	void new_player_count_selected();

	size_t                    get_player_count();
	std::string               get_game_context_name();
	cached_game_ctx_builder_t get_context_builder();

	void load_new_contexts();

	auto& get_player_count_cb() { return comboBox_selectablePlayerCount; }
	auto& get_player_config_cb() { return comboBox_definedGameContexts; }

  protected:
	virtual void on_show() override;

  private:
	const ViewContext                   ctx;
	game_ctx_builder_by_player_count_t& allGameContexts;
	size_t                              localPlayerCount;

	Gtk::Label        label_selectablePlayerCount;
	Gtk::ComboBoxText comboBox_selectablePlayerCount;

	Gtk::Label        label_selectGameType;
	Gtk::ComboBoxText comboBox_definedGameContexts;
	Gtk::Button       button_CustomConfig;

	void custom_config_button_clicked();
};

#endif // GAME_CONFIGURATION_WIDGETS_H