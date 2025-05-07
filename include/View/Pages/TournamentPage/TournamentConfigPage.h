#ifndef TOURNAMENT_CONFIG_PAGE_H
#define TOURNAMENT_CONFIG_PAGE_H

#include "LabeledWidget.h"
#include "NumberEntry.h"
#include "Page.h"
#include "SimulationConfigPage.h"
#include "Tournament.h"
#include "TournamentPage.h"

class TournamentConfigPage : public SimulationConfigPageBase {
  public:
	TournamentConfigPage(const ViewContext& _ctx);

  private:
	LabeledWidget<NumberEntry> lne_GameCount;

	std::unique_ptr<TournamentPage> tournamentPage;

	virtual void start_button_clicked() override;
};

#endif // TOURNAMENT_CONFIG_PAGE_H