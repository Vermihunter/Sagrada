#include "TournamentConfigPage.h"

#include "DefaultPointMapper.h"
#include "SagradaWindow.h"
#include "TournamentConfig.h"

TournamentConfigPage::TournamentConfigPage(const ViewContext& _ctx)
    : SimulationConfigPageBase(_ctx, "tournament"), lne_GameCount("Game count")
{
    set_size_request(ctx.size.width, ctx.size.height);

    pageElements.vbox_AllWidgets.pack_start(lne_GameCount);
    pageElements.add_all_to_vbox();

    add(pageElements.fixed);
    show_all_children();
}

void TournamentConfigPage::start_button_clicked()
{
    int gameCount;

    try {
        gameCount = lne_GameCount.widget.get_underlying_number("Game count");
    }
    catch (const std::exception& e) {
        ctx.mainWindow.display_error_message("Invalid configuration!",
                                             e.what());
        return;
    }

    auto commonGameInfo = pageElements.get_selected_information();
    // Invalid config
    if (!commonGameInfo) {
        return;
    }

    if (tournamentPage) {
        ctx.mainWindow.remove_page(*tournamentPage);
    }

    auto tournamentConfig = std::make_unique<TournamentConfig>();
    tournamentConfig->gameCount = gameCount;
    tournamentConfig->gameConfig = std::move(commonGameInfo);
    // tournamentConfig->gameConfig->randomSeed = commonGameInfo->randomSeed;
    // tournamentConfig->gameConfig->gameCtxBuilder =
    // commonGameInfo->gameCtxBuilder; tournamentConfig->gameConfig->players =
    // std::move(commonGameInfo->players);

    DefaultPointMapper::initialize_for_config(*tournamentConfig);

    tournamentPage =
        std::make_unique<TournamentPage>(ctx, std::move(tournamentConfig));
    ctx.mainWindow.add_page(*tournamentPage);
    ctx.mainWindow.show_page(*tournamentPage);
}