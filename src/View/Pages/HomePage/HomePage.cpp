#include "HomePage.h"

#include "SagradaWindow.h"

HomePage::HomePage(
    const ViewContext&
        ctx /*, game_mode_t gameMode, tournament_config_t tournamentConfig*/)
    : Page(ctx)
    , button_LocalGame("Start game")
    , button_Simulation("Start simulation")
    , button_Tournament("Start tournament")
{
    homePageLocalFixed.set_size_request(ctx.size.width, ctx.size.height);
    backgroundFixed.put(homePageLocalFixed, 0, 0);

    button_LocalGame.signal_clicked().connect(
        sigc::mem_fun(*this, &HomePage::start_local_clicked));

    button_Simulation.signal_clicked().connect(
        sigc::mem_fun(*this, &HomePage::start_simulation_clicked));

    button_Tournament.signal_clicked().connect(
        sigc::mem_fun(*this, &HomePage::start_tournament_clicked));

    starterButtonsBox.set_size_request(150, 10);
    starterButtonsBox.pack_start(button_LocalGame);
    starterButtonsBox.pack_start(button_Simulation, Gtk::PACK_SHRINK, 10);
    starterButtonsBox.pack_start(button_Tournament);
    homePageLocalFixed.put(starterButtonsBox, (ctx.size.width - 150) / 2,
                           ctx.size.height / 2);

    widgets.push_back(&homePageLocalFixed);

    add_events(Gdk::BUTTON_PRESS_MASK);
    add(backgroundFixed);

    show();
}

void HomePage::show_simulation(tournament_config_t tournamentConfig)
{
    simulationSubPage = SimulationConfigPage::with_concrete_simulation(
        ctx, std::move(tournamentConfig->gameConfig));
}

void HomePage::start_simulation_clicked()
{
    if (simulationSubPage) {
        ctx.mainWindow.remove_page(*simulationSubPage);
    }

    simulationSubPage = std::make_unique<SimulationConfigPage>(ctx);
    ctx.mainWindow.add_page(*simulationSubPage);
    ctx.mainWindow.show_page(*simulationSubPage, true);
}

void HomePage::start_local_clicked()
{
    if (!localPlayerSubpage) {
        localPlayerSubpage =
            std::make_unique<LocalPlayerGameTypeChoosingSubpage>(ctx);
        ctx.mainWindow.add_page(*localPlayerSubpage);
    }

    ctx.mainWindow.show_page(*localPlayerSubpage, true);
}

void HomePage::start_tournament_clicked()
{
    if (!tournamentConfigPage) {
        tournamentConfigPage = std::make_unique<TournamentConfigPage>(ctx);
        ctx.mainWindow.add_page(*tournamentConfigPage);
    }

    ctx.mainWindow.show_page(*tournamentConfigPage, true);
}