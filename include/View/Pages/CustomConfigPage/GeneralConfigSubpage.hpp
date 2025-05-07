#ifndef GENERALCONFIGSUBPAGE_HPP
#define GENERALCONFIGSUBPAGE_HPP

#include "CustomConfigPageSubpage.h"
#include "GeneralConfigSubPageInformation.h"
#include "LabeledWidget.h"
#include "NumberEntry.h"

class GeneralConfigSubpage : public CustomConfigPageSubpage {
  public:
	GeneralConfigSubpage(const ViewContext& ctx)
	    : CustomConfigPageSubpage(ctx), lEntry_contextName("Game mode name"),
	      lEntry_numberOfRounds("Number of rounds", std::to_string(defaultNumberOfRounds)),
	      lEntry_wpcChoiceForPlayers("Window pattern card choice for each "
	                                 "player at the beginning of the game",
	                                 std::to_string(defaultWPCChoicePerPlayer)),
	      vbox_allSettings(true)
	{
		vbox_allSettings.pack_start(lEntry_contextName);
		vbox_allSettings.pack_start(lEntry_numberOfRounds);
		vbox_allSettings.pack_start(lEntry_wpcChoiceForPlayers);

		vbox_allSettings.set_size_request(500, 200);
		put(vbox_allSettings, (ctx.size.width - 500) / 2, 80);
	}

	general_config_subpage_info_t collect_information()
	{
		return std::make_unique<GeneralConfigSubpageInformation>(
		    lEntry_contextName.widget.get_text(),
		    lEntry_numberOfRounds.widget.get_underlying_number("Number of rounds"),
		    lEntry_wpcChoiceForPlayers.widget.get_underlying_number("WPC choice per player"));
	}

  private:
	LabeledWidget<Gtk::Entry>  lEntry_contextName;
	LabeledWidget<NumberEntry> lEntry_numberOfRounds;
	LabeledWidget<NumberEntry> lEntry_wpcChoiceForPlayers;

	Gtk::VBox vbox_allSettings;
};

#endif // GENERALCONFIGSUBPAGE_HPP