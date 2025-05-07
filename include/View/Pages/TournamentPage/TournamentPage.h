#ifndef TOURNAMENT_PAGE_H
#define TOURNAMENT_PAGE_H

#include "Page.h"
#include "Tournament.h"
#include "TournamentConfig.h"
#include "TournamentGameInformation.h"
#include "TournamentResultPage.h"
#include "TournamentStatistics.h"
#include "TournamentStatisticsPage.h"

#include <gtkmm.h>
#include <vector>
class TournamentPage : public Page {
  public:
	TournamentPage(const ViewContext& _ctx, tournament_config_t _tournamentConfig);

  private:
	int remainingGames;

	size_t               currSeed;
	TournamentStatistics statistics;
	Tournament           tournament;

	Gtk::Alignment alignment_MiddleTitle;
	Gtk::Label     label_Title;

	Gtk::Fixed           fixed;
	bool                 tournamentStopped;
	TournamentResultPage results;

	Gtk::Alignment                            alignment_StatisticsPageMiddle;
	std::unique_ptr<TournamentStatisticsPage> statisticsPage;

	Gtk::HBox   hbox_DownButtons;
	Gtk::Button button_SwitchToResults;
	Gtk::Button button_SwitchToOverallStatistics;
	Gtk::Button button_Quit;

	sigc::connection firstTimeoutHandler;
	sigc::connection gameRunnerTimeoutHandler;

	ai_player_config_pc                 aiConfigs;
	std::vector<tournament_game_info_t> playedGames;
	bool                                on_first_starting_timeout();
	bool                                on_game_runner_timeout();
	void                                quit_button_clicked();

	void show_overall_statistics_button_clicked();
	void show_game_results_button_clicked();
	void change_shown_part(Gtk::Widget* from, Gtk::Widget* to);
};

#endif // TOURNAMENT_PAGE_H