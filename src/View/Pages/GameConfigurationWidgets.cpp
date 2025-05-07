#include "GameConfigurationWidgets.h"

#include "HomePageSubpage.h"
#include "SagradaWindow.h"

GameConfigurationWidgets::GameConfigurationWidgets(
    const ViewContext& _ctx, game_ctx_builder_by_player_count_t& allCtxBuilders,
    size_t _localPlayerCount)
    : ctx(_ctx), allGameContexts(allCtxBuilders), localPlayerCount(_localPlayerCount),
      label_selectablePlayerCount("Select AI player count"),
      label_selectGameType("Select game configuration"),
      button_CustomConfig("Create custom configuration")
{
	for (auto&& [playerCount, gameCtx] : allGameContexts) {
		comboBox_selectablePlayerCount.append(std::to_string(playerCount - localPlayerCount));
	}

	comboBox_selectablePlayerCount.signal_changed().connect(
	    sigc::mem_fun(*this, &GameConfigurationWidgets::new_player_count_selected));

	comboBox_selectablePlayerCount.set_active_text("2");
	comboBox_definedGameContexts.set_active_text("default");

	button_CustomConfig.signal_clicked().connect(
	    sigc::mem_fun(*this, &GameConfigurationWidgets::custom_config_button_clicked));

	pack_start(label_selectablePlayerCount);
	pack_start(comboBox_selectablePlayerCount);
	pack_start(label_selectGameType);
	pack_start(comboBox_definedGameContexts);
	pack_start(button_CustomConfig);
}

void GameConfigurationWidgets::on_show()
{
	load_new_contexts();
	Gtk::VBox::on_show();
}

void GameConfigurationWidgets::load_new_contexts()
{
	for (auto&& [playerCount, ctxBuilder] : allGameContexts) {
		const std::string playerCountStr = std::to_string(playerCount - localPlayerCount);

		bool containsString = false;
		for (const auto& child : comboBox_selectablePlayerCount.get_model()->children()) {
			Gtk::TreeModel::Row row = *child;
			std::string         itemText;
			row.get_value(0, itemText); // Assuming there's only one column

			if (itemText == playerCountStr) {
				containsString = true;
				break;
			}
		}

		if (!containsString) {
			comboBox_selectablePlayerCount.append(playerCountStr);
		}
	}
}

void GameConfigurationWidgets::new_player_count_selected()
{
	size_t selectedPlayerCount = std::stoul(comboBox_selectablePlayerCount.get_active_text());
	comboBox_definedGameContexts.remove_all();

	for (auto&& gameBuilder : allGameContexts[selectedPlayerCount + localPlayerCount]) {
		comboBox_definedGameContexts.append(gameBuilder->get_name());
	}

	comboBox_definedGameContexts.set_active(0);
}

size_t GameConfigurationWidgets::get_player_count()
{
	return std::stoul(comboBox_selectablePlayerCount.get_active_text());
}

std::string GameConfigurationWidgets::get_game_context_name()
{
	return comboBox_definedGameContexts.get_active_text();
}

cached_game_ctx_builder_t GameConfigurationWidgets::get_context_builder()
{
	size_t      playerCount = get_player_count() + localPlayerCount;
	std::string gameCtxName = get_game_context_name();

	auto& gameCtxBySelectedPlayerCount = GameCache::get().get_defined_game_contexts()[playerCount];
	auto  gameCtxIt                    = std::find_if(
	                        gameCtxBySelectedPlayerCount.begin(), gameCtxBySelectedPlayerCount.end(),
	                        [&](const auto& gameBuilderCtx) { return gameBuilderCtx->get_name() == gameCtxName; });

	if (gameCtxIt == gameCtxBySelectedPlayerCount.end()) {
		throw std::invalid_argument{"Invalid game context!"};
	}

	return *gameCtxIt;
}

void GameConfigurationWidgets::custom_config_button_clicked()
{
	ctx.mainWindow.show_custom_config_page();
}