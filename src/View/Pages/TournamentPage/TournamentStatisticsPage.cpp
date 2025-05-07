#include "TournamentStatisticsPage.h"

#include <ostream>

TournamentStatisticsPage::TournamentStatisticsPage(const ViewContext&    _ctx,
                                                   TournamentStatistics& _tournamentStat)
    : ctx(_ctx), tournamentStat(_tournamentStat),
      label_OverallStatisticsTitle("Overall statistics"),
      label_TotalGameCountTitle("Total game count"),
      label_TotalGameCount(std::to_string(tournamentStat.gameResults.size())),
      label_PlayerStatisticsTitle("Player statistics"), label_PlayerName("AI player type"),
      label_WinCount("Win count"), label_ReceivedPlacementPoints("Placement points"),
      label_AveragePoints("Average points")
{
	set_size_request(ctx.size.width, -1);

	pageCss = Gtk::CssProvider::create();
	try {
		pageCss->load_from_path("data/Css/TournamentStatisticsPage.css");
	}
	catch (const Glib::Error& ex) {
		std::cerr << "CSS loading error: " << ex.what() << std::endl;
	}

	AI_PlayerConfig*    tournamentWinner    = tournamentStat.total_winner();
	WilsonScoreInterval wilsonScoreInterval = tournamentStat.playerStatistics.at(tournamentWinner)
	                                              .get_wilson_score_interval_with_98_confidence();

	std::ostringstream wilsonScoreTextOss;
	wilsonScoreTextOss << tournamentWinner->get_name() << " wins " << std::setprecision(3)
	                   << wilsonScoreInterval.interval_low * 100 << "% - "
	                   << wilsonScoreInterval.interval_high * 100 << "% games with "
	                   << wilsonScoreInterval.confidence << "% confidence";
	label_WilsonScoreIntervalForWinningPlayer.set_text(wilsonScoreTextOss.str());
	label_WilsonScoreIntervalForWinningPlayer.get_style_context()->add_provider(
	    pageCss, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	label_WilsonScoreIntervalForWinningPlayer.set_name("tournament_header_label");
	label_WilsonScoreIntervalForWinningPlayer.set_halign(Gtk::ALIGN_CENTER);

	grid_PlayerStatistics.get_style_context()->add_provider(
	    pageCss, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	grid_PlayerStatistics.set_halign(Gtk::ALIGN_CENTER);
	grid_OverallStatistics.get_style_context()->add_provider(
	    pageCss, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	grid_OverallStatistics.set_halign(Gtk::ALIGN_CENTER);
	initialize_table();

	pack_start(grid_PlayerStatistics);
	pack_start(grid_OverallStatistics);
	pack_start(label_WilsonScoreIntervalForWinningPlayer);

	show_all_children();
}

void TournamentStatisticsPage::add_player_statistics(const AI_PlayerConfig*  aiConfig,
                                                     const PlayerStatistics& playerStat,
                                                     size_t                  playerInd)
{
	playerStatisticsWidgets.push_back(PlayerStatisticsWidgets{aiConfig, playerStat, pageCss});

	PlayerStatisticsWidgets& playerStatWidgets = playerStatisticsWidgets.back();

	grid_PlayerStatistics.attach(playerStatWidgets.label_PlayerName, 0, playerInd + 2);
	grid_PlayerStatistics.attach(playerStatWidgets.label_WinCount, 1, playerInd + 2);
	grid_PlayerStatistics.attach(playerStatWidgets.label_ReceivedPlacementPoints, 2, playerInd + 2);
	grid_PlayerStatistics.attach(playerStatWidgets.label_AveragePoints, 3, playerInd + 2);
}

void TournamentStatisticsPage::initialize_table()
{
	label_ReceivedPlacementPoints.set_tooltip_text(
	    "Each player gets points for every game. Currently the point system "
	    "awards the players "
	    "with a score having the opposite "
	    "value of their placement according to the number of players. This "
	    "means that the 1st "
	    "place in a 3 member game gets 3 points, the "
	    "2nd place gets 2 points and the 3rd place gets a single point");

	constexpr size_t overallTableTitleRow = 0;
	constexpr size_t columnTitleLabelRow  = 1;

	initialize_column_with_css(grid_PlayerStatistics, label_PlayerStatisticsTitle,
	                           overallTableTitleRow, 0, 4, true);
	initialize_column_with_css(grid_PlayerStatistics, label_PlayerName, columnTitleLabelRow, 0, 1,
	                           true);
	initialize_column_with_css(grid_PlayerStatistics, label_WinCount, columnTitleLabelRow, 1, 1,
	                           true);
	initialize_column_with_css(grid_PlayerStatistics, label_ReceivedPlacementPoints,
	                           columnTitleLabelRow, 2, 1, true);
	initialize_column_with_css(grid_PlayerStatistics, label_AveragePoints, columnTitleLabelRow, 3,
	                           1, true);

	size_t ind = 0;
	for (auto&& [aiConfig, aiStat] : tournamentStat.playerStatistics) {
		add_player_statistics(aiConfig, aiStat, ind++);
	}

	initialize_column_with_css(grid_OverallStatistics, label_OverallStatisticsTitle,
	                           overallTableTitleRow, 0, 2, true);
	initialize_column_with_css(grid_OverallStatistics, label_TotalGameCountTitle, 1, 0, 1, true);
	initialize_column_with_css(grid_OverallStatistics, label_TotalGameCount, 1, 1, 1, false);
}

void TournamentStatisticsPage::initialize_column_with_css(Gtk::Grid&  grid,
                                                          Gtk::Label& label_ColumnTitle,
                                                          size_t index_Row, size_t index_Column,
                                                          size_t width_Column, bool isHeader)
{
	grid.attach(label_ColumnTitle, index_Column, index_Row, width_Column);
	if (isHeader) {
		label_ColumnTitle.set_name("tournament_header_label");
	}
	label_ColumnTitle.get_style_context()->add_provider(pageCss,
	                                                    GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}