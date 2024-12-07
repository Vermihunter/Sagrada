#include "SimulationConfigPage.h"

#include "SagradaWindow.h"

SimulationConfigPage::SimulationConfigPage(const ViewContext& _ctx)
    : SimulationConfigPageBase(_ctx, "simulation")
{
    pageElements.add_all_to_vbox();

    add(pageElements.fixed);
    show_all_children();
}

void SimulationConfigPage::start_button_clicked()
{
    auto gameConfig = pageElements.get_selected_information();
    // Invalid config
    if (!gameConfig) {
        return;
    }

    if (gamePlayingPage) {
        ctx.mainWindow.remove_page(*gamePlayingPage);
    }

    start_simulation_with_config(std::move(gameConfig));
}

void SimulationConfigPage::start_simulation_with_config(
    game_config_t gameConfig)
{
    auto gameCtx = gameConfig->gameCtxBuilder->build(
        gameConfig->players.size(),
        RandomGenerator::initialize_with_seed(gameConfig->randomSeed));
    auto game = std::make_unique<Game>(
        std::move(gameCtx), DefaultGameCreator::get_default_game_state());

    ai_player_config_pc tmpConfigs;
    for (auto&& playerConfig : gameConfig->players) {
        tmpConfigs.push_back(playerConfig.get());
    }
    auto controller = std::make_unique<OnlyAIPlayerController>(
        *game, tmpConfigs, gameConfig->gameDeterministic);
    gamePlayingPage = std::make_unique<SimulationGamePlayingPage>(
        ctx.mainWindow.homePage, true, ctx, std::move(controller),
        std::move(game));

    gamePlayingPage->hide_non_load_page_widgets();
    ctx.mainWindow.add_page(*gamePlayingPage);
    ctx.mainWindow.show_page(*gamePlayingPage);
}

std::unique_ptr<SimulationConfigPage>
SimulationConfigPage::with_concrete_simulation(const ViewContext& _ctx,
                                               game_config_t gameConfig)
{
    std::unique_ptr<SimulationConfigPage> simulationPage =
        std::make_unique<SimulationConfigPage>(_ctx);
    simulationPage->start_simulation_with_config(std::move(gameConfig));

    return simulationPage;
}