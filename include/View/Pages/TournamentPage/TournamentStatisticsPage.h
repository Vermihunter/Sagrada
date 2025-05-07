#ifndef TOURNAMENT_STATISTICS_PAGE_H
#define TOURNAMENT_STATISTICS_PAGE_H

#include "PlayerStatisticsWidgets.h"
#include "TournamentStatistics.h"
#include "ViewContext.h"

#include <gtkmm.h>
#include <vector>

class TournamentStatisticsPage : public Gtk::VBox {
  public:
	TournamentStatisticsPage(const ViewContext& _ctx, TournamentStatistics& _tournamentStat);

  private:
	ViewContext           ctx;
	TournamentStatistics& tournamentStat;

	// Overall statistics widgets
	Gtk::Grid  grid_OverallStatistics;
	Gtk::Label label_OverallStatisticsTitle;
	Gtk::Label label_TotalGameCountTitle;
	Gtk::Label label_TotalGameCount;
	Gtk::Label label_WilsonInterval;

	// Player statistics widgets
	std::vector<PlayerStatisticsWidgets> playerStatisticsWidgets;
	Gtk::Grid                            grid_PlayerStatistics;
	Gtk::Label                           label_PlayerStatisticsTitle;
	Gtk::Label                           label_PlayerName;
	Gtk::Label                           label_WinCount;
	Gtk::Label                           label_ReceivedPlacementPoints;
	Gtk::Label                           label_AveragePoints;

	// Wilson interval
	Gtk::Label label_WilsonScoreIntervalForWinningPlayer;

	Glib::RefPtr<Gtk::CssProvider> pageCss;

	void add_player_statistics(const AI_PlayerConfig* aiConfig, const PlayerStatistics& playerStat,
	                           size_t playerInd);
	void initialize_table();
	void initialize_column_with_css(Gtk::Grid& grid, Gtk::Label& label_ColumnTitle,
	                                size_t index_Row, size_t index_Column, size_t width_Column,
	                                bool isHeader);
};

#endif // TOURNAMENT_STATISTICS_PAGE_H