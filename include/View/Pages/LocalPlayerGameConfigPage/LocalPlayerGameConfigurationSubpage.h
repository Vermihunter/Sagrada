#ifndef LOCAL_PLAYER_GAME_CONFIGURATION_SUBPAGE_H
#define LOCAL_PLAYER_GAME_CONFIGURATION_SUBPAGE_H

#include "GameCache.h"
#include "GameConfigurationWidgets.h"
#include "Image.h"
#include "LabeledWidget.h"
#include "NumberEntry.h"
#include "Page.h"
#include "PageType.h"
#include "RandomGenerator.h"
#include "SimulationConfigPageBase.h"
#include "WPCChoosingPage.h"

#include <memory>

class LocalPlayerGameConfigurationSubpage : public SimulationConfigPageBase {
  public:
	LocalPlayerGameConfigurationSubpage(const ViewContext& ctx);

  protected:
	virtual void start_button_clicked() override;

  private:
	std::unique_ptr<WPCChoosingPage> wpcChoosingPage;

	void display_invalid_config_error_message(const std::string& message);
};

#endif // LOCAL_PLAYER_GAME_CONFIGURATION_SUBPAGE_H