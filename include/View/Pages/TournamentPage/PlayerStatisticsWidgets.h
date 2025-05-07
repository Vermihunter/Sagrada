#ifndef PLAYER_STATISTICS_WIDGETS
#define PLAYER_STATISTICS_WIDGETS

#include "AI_PlayerConfig.h"
#include "TournamentStatistics.h"

#include <gtkmm.h>

class PlayerStatisticsWidgets {
  public:
	PlayerStatisticsWidgets(const AI_PlayerConfig* _aiConfig, const PlayerStatistics& _playerStat,
	                        Glib::RefPtr<Gtk::CssProvider>& css)
	    : label_PlayerName(_aiConfig->get_name()),
	      label_WinCount(std::to_string(_playerStat.winCount)),
	      label_ReceivedPlacementPoints(std::to_string(_playerStat.totalPlacementPoints)),
	      label_AveragePoints(std::to_string((double)(_playerStat.totalReceivedPoints) /
	                                         (double)(_playerStat.statistics.gameResults.size())))
	{
		label_PlayerName.get_style_context()->add_provider(css,
		                                                   GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
		label_WinCount.get_style_context()->add_provider(css,
		                                                 GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
		label_ReceivedPlacementPoints.get_style_context()->add_provider(
		    css, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
		label_AveragePoints.get_style_context()->add_provider(
		    css, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	}

	Gtk::Label label_PlayerName;
	Gtk::Label label_WinCount;
	Gtk::Label label_ReceivedPlacementPoints;
	Gtk::Label label_AveragePoints;
};

#endif // PLAYER_STATISTICS_WIDGETS