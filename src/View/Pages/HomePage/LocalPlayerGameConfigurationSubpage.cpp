#include "LocalPlayerGameConfigurationSubpage.h"

#include "SagradaWindow.h"

LocalPlayerGameConfigurationSubpage::LocalPlayerGameConfigurationSubpage(const ViewContext& ctx)
    : SimulationConfigPageBase(ctx, "local game", 1)
{
	pageElements.add_all_to_vbox();

	add(pageElements.fixed);
	show_all_children();
}

void LocalPlayerGameConfigurationSubpage::display_invalid_config_error_message(
    const std::string& message)
{
	ctx.mainWindow.display_error_message("Invalid config", message);
}

void LocalPlayerGameConfigurationSubpage::start_button_clicked()
{
	// Hard coded 1 local player count
	constexpr size_t localPlayerCount = 1;

	auto commonGameInfo = pageElements.get_selected_information(true);
	// Invalid config
	if (!commonGameInfo) {
		return;
	}

	rnd_t rnd = (pageElements.lne_Seed.widget.get_text().empty())
	                ? RandomGenerator::initialize_with_curr_time()
	                : RandomGenerator::initialize_with_seed(commonGameInfo->randomSeed);

	auto gameCtx = commonGameInfo->gameCtxBuilder->build(
	    commonGameInfo->players.size() + localPlayerCount, std::move(rnd));
	auto game =
	    std::make_unique<Game>(std::move(gameCtx), DefaultGameCreator::get_default_game_state());

	// Removing previous wpc choosing page
	if (wpcChoosingPage) {
		ctx.mainWindow.remove_page(*wpcChoosingPage);
	}

	wpcChoosingPage = std::make_unique<WPCChoosingPage>(ctx, std::move(game),
	                                                    commonGameInfo->players, localPlayerCount);

	ctx.mainWindow.add_page(*wpcChoosingPage);
	ctx.mainWindow.show_page(*wpcChoosingPage);
}