#include "TournamentResultPage.h"

#include "SagradaWindow.h"
#include "TournamentPage.h"

TournamentResultPage::TournamentResultPage(TournamentPage& _parentPage, const ViewContext& _ctx,
                                           size_t _resultsPerPage)
    : parentPage(_parentPage), ctx(_ctx), resultsPerPage(_resultsPerPage), hbox_ColumnNames(true),
      label_GameCountTitle("Game count"), label_UsedSeedTitle("Used seed"),
      label_GameWinnerTitle("Game winner"), label_SimulateButtonTitle("Start simulation")
{
	hbox_ColumnNames.set_size_request(ctx.size.width - 10, 50);
	hbox_ColumnNames.pack_start(label_GameCountTitle);
	hbox_ColumnNames.pack_start(label_UsedSeedTitle);
	hbox_ColumnNames.pack_start(label_GameWinnerTitle);
	hbox_ColumnNames.pack_start(label_SimulateButtonTitle);
	frame_ColumnNames.add(hbox_ColumnNames);

	vbox_GameResultsTable.pack_start(frame_ColumnNames);
	sw_GameResults.add(vbox_GameResultsTable);
	sw_GameResults.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	sw_GameResults.set_size_request(ctx.size.width, ctx.size.height - 80);

	put(sw_GameResults, 0, 0);
}

void TournamentResultPage::add_game_result(const TournamentGameInformation& gameInfo,
                                           size_t                           gameCount)
{
	gameResults.push_back(std::make_unique<TournamentGameResult>(
	    parentPage, ctx, ctx.with_reduced_height(1 / 0.9), gameInfo, gameCount));
	gameResults.back()->set_size_request(ctx.size.width - 10,
	                                     (ctx.size.height - 55 - 70) / resultsPerPage);
	vbox_GameResultsTable.pack_start(*gameResults.back(), Gtk::PACK_EXPAND_PADDING);

	gameResults.back()->show();
}

TournamentResultPage::~TournamentResultPage() {}