#ifndef SIMULATION_CONFIG_BASE_H
#define SIMULATION_CONFIG_BASE_H

#include "CommonGameConfigElements.h"
#include "GameCache.h"
#include "GamePlayingPage.h"
#include "LabeledWidget.h"
#include "NumberEntry.h"
#include "Page.h"
#include "RandomGenerator.h"
#include "SimulationGamePlayingPage.h"

class SimulationConfigPageBase : public Page
{
  public:
    SimulationConfigPageBase(const ViewContext& ctx,
                             const std::string& pageName,
                             size_t localPlayerCount = 0);

  protected:
    virtual void on_show() override
    {
        Page::on_show();
        pageElements.gameCtxWidgets.load_new_contexts();
    }

    CommonGameConfigElements pageElements;

    virtual void start_button_clicked() = 0;
};

#endif // SIMULATION_CONFIG_BASE_H