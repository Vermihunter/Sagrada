#include "TournamentPage.h"

#include "SagradaWindow.h"

TournamentPage::TournamentPage(const ViewContext& _ctx,
                               tournament_config_t _tournamentConfig)
    : Page(_ctx)
    , remainingGames(_tournamentConfig->gameCount)
    , currSeed(_tournamentConfig->gameConfig->randomSeed)
    , statistics(_tournamentConfig->gameConfig->players)
    , tournament(std::move(_tournamentConfig), statistics)
    , tournamentStopped(false)
    , results(*this, ctx.with_reduced_height(0.90), 10)
    , alignment_StatisticsPageMiddle(0.5, 0.0)
    , button_SwitchToResults("Show results")
    , button_SwitchToOverallStatistics("Show overall statistics")
    , button_Quit("Quit")
{
    std::string title = "Tournament between ";
    title +=
        tournament.get_config()->gameConfig->players[0]->get_name() + " player";

    bool isFirst = true;
    for (auto&& aiPlayer : tournament.get_config()->gameConfig->players) {
        if (isFirst) {
            isFirst = false;
            continue;
        }

        title += (" and " + aiPlayer->get_name() + " player");
    }
    Pango::FontDescription font_desc;
    font_desc.set_style(Pango::STYLE_NORMAL);
    font_desc.set_weight(Pango::WEIGHT_BOLD);
    font_desc.set_size(Pango::SCALE * 18); // Adjust the size as needed

    // Set the font description on the Gtk::Label
    label_Title.override_font(font_desc);
    label_Title.set_text(title);

    alignment_MiddleTitle.set_size_request(ctx.size.width,
                                           ctx.size.height * 0.10);
    alignment_MiddleTitle.add(label_Title);

    button_Quit.signal_clicked().connect(
        sigc::mem_fun(*this, &TournamentPage::quit_button_clicked));
    button_Quit.set_size_request(200, 50);

    button_SwitchToOverallStatistics.signal_clicked().connect(sigc::mem_fun(
        *this, &TournamentPage::show_overall_statistics_button_clicked));

    button_SwitchToOverallStatistics.set_sensitive(false);
    button_SwitchToOverallStatistics.set_size_request(200, 50);

    button_SwitchToResults.signal_clicked().connect(sigc::mem_fun(
        *this, &TournamentPage::show_game_results_button_clicked));
    button_SwitchToResults.set_size_request(200, 50);

    hbox_DownButtons.pack_start(button_Quit);
    hbox_DownButtons.pack_start(button_SwitchToOverallStatistics);

    fixed.put(alignment_MiddleTitle, 0, 0);
    fixed.put(results, 0, ctx.size.height * 0.10);
    fixed.put(hbox_DownButtons, (ctx.size.width - 400) / 2,
              ctx.size.height - 50 - 5);
    add(fixed);
    show_all_children();

    constexpr unsigned int timeOutInMs = 150;
    firstTimeoutHandler = Glib::signal_timeout().connect(
        sigc::mem_fun(*this, &TournamentPage::on_first_starting_timeout),
        timeOutInMs);

    for (auto&& player : tournament.get_config()->gameConfig->players) {
        aiConfigs.push_back(player.get());
    }
}

bool TournamentPage::on_game_runner_timeout()
{
    if (tournamentStopped) {
        return false;
    }

    size_t gameCount = tournament.get_config()->gameCount - remainingGames + 1;
    playedGames.push_back(
        tournament.launch_game(currSeed++, gameCount, aiConfigs));
    results.add_game_result(*playedGames.back(), gameCount);

    bool allGamesRun = (--remainingGames == 0);
    if (allGamesRun) {
        statisticsPage =
            std::make_unique<TournamentStatisticsPage>(ctx, statistics);
        fixed.put(*statisticsPage, 0, ctx.size.height / 4);
        statisticsPage->hide();
        button_SwitchToOverallStatistics.set_sensitive(true);
        hbox_DownButtons.pack_start(button_SwitchToResults);
        button_SwitchToResults.hide();
    }

    return !allGamesRun;
}

bool TournamentPage::on_first_starting_timeout()
{
    constexpr unsigned int timeOutInMs = 80;
    gameRunnerTimeoutHandler = Glib::signal_timeout().connect(
        sigc::mem_fun(*this, &TournamentPage::on_game_runner_timeout),
        timeOutInMs);

    return false;
}

void TournamentPage::show_overall_statistics_button_clicked()
{
    change_shown_part(&results, statisticsPage.get());
    button_SwitchToOverallStatistics.hide();
    button_SwitchToResults.show();
}

void TournamentPage::show_game_results_button_clicked()
{
    change_shown_part(statisticsPage.get(), &results);
    button_SwitchToResults.hide();
    button_SwitchToOverallStatistics.show();
}

void TournamentPage::change_shown_part(Gtk::Widget* from, Gtk::Widget* to)
{
    from->hide();
    to->show();
}

void TournamentPage::quit_button_clicked()
{
    tournamentStopped = true;
    ctx.mainWindow.show_home_page();
}
