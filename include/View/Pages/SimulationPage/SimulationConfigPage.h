#ifndef SIMULATION_CONFIG_PAGE_H
#define SIMULATION_CONFIG_PAGE_H

#include "SimulationConfigPageBase.h"

class SimulationConfigPage : public SimulationConfigPageBase {
  public:
	SimulationConfigPage(const ViewContext& _ctx);
	static std::unique_ptr<SimulationConfigPage> with_concrete_simulation(const ViewContext& _ctx,
	                                                                      game_config_t gameConfig);

  private:
	std::unique_ptr<SimulationGamePlayingPage> gamePlayingPage;

	virtual void start_button_clicked() override;
	void         start_simulation_with_config(game_config_t gameConfig);
};

#endif // SIMULATION_CONFIG_PAGE_H