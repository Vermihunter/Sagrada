#ifndef HOME_PAGE_H
#define HOME_PAGE_H

#include "GameMode.h"
#include "Image.h"
#include "LocalPlayerGameTypeChoosingSubpage.h"
#include "Page.h"
#include "SimulationConfigPage.h"
#include "TournamentConfigPage.h"
#include "ViewConstants.h"

#include <concepts>
#include <memory>

class HomePage : public Page {
  public:
	HomePage(const ViewContext& ctx);

	void show_simulation(tournament_config_t tournamentConfig);

  private:
	Gtk::Fixed homePageLocalFixed;
	Gtk::Fixed backgroundFixed;

	Gtk::VBox   starterButtonsBox;
	Gtk::Button button_LocalGame;
	Gtk::Button button_Simulation;
	Gtk::Button button_Tournament;

	std::unique_ptr<SimulationConfigPage>               simulationSubPage;
	std::unique_ptr<LocalPlayerGameTypeChoosingSubpage> localPlayerSubpage;
	std::unique_ptr<TournamentConfigPage>               tournamentConfigPage;

	void start_simulation_clicked();
	void start_local_clicked();
	void start_tournament_clicked();
};

using home_page_t = std::unique_ptr<HomePage>;

#endif // HOME_PAGE_H