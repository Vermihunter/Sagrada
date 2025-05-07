#ifndef TOURNAMENT_RESULT_PAGE_H
#define TOURNAMENT_RESULT_PAGE_H

#include "LabeledWidget.h"
#include "NumberEntry.h"
#include "TournamentGameInformation.h"
#include "TournamentGameResult.h"
#include "ViewContext.h"

#include <gtkmm.h>
#include <vector>

class TournamentPage;

class TournamentResultPage : public Gtk::Fixed {
  public:
	TournamentResultPage(TournamentPage& _parentPage, const ViewContext& _ctx,
	                     size_t _resultsPerPage);
	~TournamentResultPage();

	void add_game_result(const TournamentGameInformation&, size_t gameCount);

  private:
	TournamentPage& parentPage;
	ViewContext     ctx;
	size_t          resultsPerPage;

	Gtk::Frame frame_ColumnNames;
	Gtk::HBox  hbox_ColumnNames;
	Gtk::Label label_GameCountTitle;
	Gtk::Label label_UsedSeedTitle;
	Gtk::Label label_GameWinnerTitle;
	Gtk::Label label_SimulateButtonTitle;

	// This will be sorted, no additional currently shown list neeeded
	std::vector<std::unique_ptr<TournamentGameResult>> gameResults;

	Gtk::ScrolledWindow sw_GameResults;
	Gtk::VBox           vbox_GameResultsTable;
};

#endif // TOURNAMENT_RESULT_PAGE_H