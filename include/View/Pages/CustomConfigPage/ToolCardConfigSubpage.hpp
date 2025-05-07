#ifndef TOOLCARDCONFIGSUBPAGE_HPP
#define TOOLCARDCONFIGSUBPAGE_HPP

#include "CustomConfigPageSubpage.h"
#include "LabeledWidget.h"
#include "NumberEntry.h"
#include "ToolCardConfig.h"
#include "ToolCardGameContext.h"
#include "ToolCardImages.h"

#include <memory>
#include <vector>

class ToolCardConfigSubpage : public CustomConfigPageSubpage {
	constexpr static size_t configPerRow = 6;

  public:
	ToolCardConfigSubpage(const ViewContext& ctx)
	    : CustomConfigPageSubpage(ctx), alignment_CardPacksMiddle(0.5, 0, 0),
	      lne_CardsPerGame("Cards per game"), alignment_CardPerGameMiddle(0.5, 0, 0)
	{
		size_t ind = 0;
		for (auto&& tcImage : ToolCardImages::get_all()) {
			tcConfigs.push_back(std::make_unique<ToolCardConfig>(*tcImage, tc_etype(ind)));

			if (ind < configPerRow) {
				hbox_CardsPack1.pack_start(*tcConfigs.back(), Gtk::PACK_SHRINK, 5);
			}
			else {
				hbox_CardsPack2.pack_start(*tcConfigs.back(), Gtk::PACK_SHRINK, 5);
			}

			++ind;
		}

		vbox_AllCards.pack_start(hbox_CardsPack1);
		vbox_AllCards.pack_start(hbox_CardsPack2, Gtk::PACK_SHRINK, 15);

		alignment_CardPacksMiddle.add(vbox_AllCards);
		alignment_CardPacksMiddle.set_size_request(ctx.size.width, -1);

		alignment_CardPerGameMiddle.add(lne_CardsPerGame);
		alignment_CardPerGameMiddle.set_size_request(ctx.size.width, -1);

		auto defaultTcCtx = DefaultGameCreator::get_default_tc_context();
		lne_CardsPerGame.widget.set_text(std::to_string(defaultTcPerGame));

		put(alignment_CardPacksMiddle, 0, 10);
		put(alignment_CardPerGameMiddle, 0, ctx.size.height - 120);
	}

	tc_context_t get_underlying_context()
	{
		return std::make_unique<ToolCardGameContext>(
		    lne_CardsPerGame.widget.get_underlying_number("TC per game"),
		    get_selected_card_configs());
	}

	tc_config_c get_selected_card_configs()
	{
		auto selectedFilter = std::views::filter(
		    [](const std::unique_ptr<ToolCardConfig>& obj) { return obj->is_selected(); });

		auto tcConfigGetter = std::views::transform(
		    [](const std::unique_ptr<ToolCardConfig>& obj) { return obj->type; });

		tc_config_c selectedConfigs;

		std::ranges::copy(tcConfigs | selectedFilter | tcConfigGetter,
		                  std::back_inserter(selectedConfigs));

		return selectedConfigs;
	}

  private:
	std::vector<std::unique_ptr<ToolCardConfig>> tcConfigs;

	Gtk::HBox hbox_CardsPack1;
	Gtk::HBox hbox_CardsPack2;

	Gtk::Alignment alignment_CardPacksMiddle;

	LabeledWidget<NumberEntry> lne_CardsPerGame;
	Gtk::Alignment             alignment_CardPerGameMiddle;

	Gtk::VBox vbox_AllCards;
};

#endif // TOOLCARDCONFIGSUBPAGE_HPP