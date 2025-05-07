#include "BoardConfigSubpage.hpp"

#include "SagradaWindow.h"

BoardConfigSubpage::BoardConfigSubpage(const ViewContext& ctx)
    : CustomConfigPageSubpage(ctx), label_Dimensions("Choose board dimensions!"), vbox_Width(true),
      label_Width("Width"), vbox_Height(true), label_Height("Height"), hbox_Dimensions(true),
      label_BoardTypeChooser("Choose a board type!"), vbox_BoardTypeChooser(true),
      button_ConfigureBoardOptions("Select boards by current config"), vbox_Config(true),
      hbox_Pager(true), button_Next("Next"), button_Prev("Prev"), currPageNumber(0),
      currBoardConfig(nullptr)
{
	/** Dimensions */
	vbox_Width.pack_start(label_Width);
	vbox_Width.pack_start(nEntry_Width);
	nEntry_Width.set_text(std::to_string(defaultBoardWidth));

	vbox_Height.pack_start(label_Height);
	vbox_Height.pack_start(nEntry_Height);
	nEntry_Height.set_text(std::to_string(defaultBoardHeight));

	hbox_Dimensions.pack_start(vbox_Width);
	hbox_Dimensions.pack_start(vbox_Height);

	/** Board type */
	for (auto&& [boardTypeName, boardTypePtr] : BoardType::get_all_board_types()) {
		cb_BoardTypeChooser.append(boardTypeName);
	}
	cb_BoardTypeChooser.set_active_text(defaultBoardType);
	vbox_BoardTypeChooser.pack_start(label_BoardTypeChooser);
	vbox_BoardTypeChooser.pack_start(cb_BoardTypeChooser);

	vbox_BoardTypeChooser.set_size_request(300, 60);

	button_ConfigureBoardOptions.signal_clicked().connect(
	    sigc::mem_fun(*this, &BoardConfigSubpage::show_config_button_clicked));
	button_ConfigureBoardOptions.set_margin_top(30);

	/** Config together */
	vbox_Config.pack_start(label_Dimensions);
	vbox_Config.pack_start(hbox_Dimensions);
	vbox_Config.pack_start(vbox_BoardTypeChooser);
	vbox_Config.pack_start(button_ConfigureBoardOptions);

	frame_Config.add(vbox_Config);
	frame_Config.set_size_request(300, 175);
	put(frame_Config, (ctx.size.width - 300) / 2, 30);
	frame_Config.show();

	/** Pager settings */
	button_Prev.signal_clicked().connect(
	    sigc::bind(sigc::mem_fun(*this, &BoardConfigSubpage::change_page_number), -1));

	button_Next.signal_clicked().connect(
	    sigc::bind(sigc::mem_fun(*this, &BoardConfigSubpage::change_page_number), 1));

	hbox_Pager.pack_start(button_Prev);
	hbox_Pager.pack_start(button_Next);
	hbox_Pager.set_size_request(150, 40);
	button_Prev.set_sensitive(false);
	button_Next.set_sensitive(false);

	put(hbox_Pager, (ctx.size.width - 150) / 2, ctx.size.height - 80);
	// show_all_children();
}

void BoardConfigSubpage::on_show()
{
	Gtk::Fixed::on_show();
	hbox_Pager.hide();
}

void BoardConfigSubpage::change_page_number(int changeVal)
{
	currPageNumber += changeVal;

	button_Prev.set_sensitive((currPageNumber > 0));
	button_Next.set_sensitive(
	    (size_t(currPageNumber + 1) < (selectableWPCBySelectedConfig.size() / wpcChoicePerPage)));

	for (size_t i = 0; i < wpcChoicePerPage; ++i) {
		size_t prevInd = (currPageNumber - changeVal) * wpcChoicePerPage + i;
		size_t nextInd = currPageNumber * wpcChoicePerPage + i;

		if (prevInd >= 0 && prevInd < selectableWPCBySelectedConfig.size()) {
			selectableWPCBySelectedConfig[prevInd].hide();
		}

		if (nextInd >= 0 && nextInd < selectableWPCBySelectedConfig.size()) {
			selectableWPCBySelectedConfig[nextInd].show();
		}
	}
}

BoardConfig* BoardConfigSubpage::get_board_config()
{
	if (cb_BoardTypeChooser.get_active_row_number() == -1) {
		throw InformationNotProvidedException{"Select a board type!"};
	}

	BoardType* boardType = BoardType::get_type(cb_BoardTypeChooser.get_active_text());

	if (!boardType) {
		throw InformationNotProvidedException{"Invalid board type"};
	}

	size_t boardWidth  = nEntry_Width.get_underlying_number("Board width");
	size_t boardHeight = nEntry_Height.get_underlying_number("Board height");

	return BoardConfig::get_config(boardWidth, boardHeight, boardType);
}

void BoardConfigSubpage::show_config_button_clicked()
{
	BoardConfig* boardConfig;

	try {
		boardConfig = get_board_config();
	}
	catch (const InformationNotProvidedException& e) {
		return display_error_message(e.what());
	}

	if (!boardConfig || currBoardConfig == boardConfig) {
		return;
	}

	currBoardConfig = boardConfig;

	if (!selectableWPCBySelectedConfig.empty()) {
		for (auto&& testView : selectableWPCBySelectedConfig) {
			remove(testView);
		}
		selectableWPCBySelectedConfig.clear();
	}

	auto& allWpc = GameCache::get().get_wpc();
	auto  wpcIt  = allWpc.find(currBoardConfig);

	if (wpcIt == allWpc.end()) {
		return display_error_message(
		    "No available window pattern card exists for this cofniguration!");
	}

	create_wpc(wpcIt->second);

	if (selectableWPCBySelectedConfig.empty()) {
		return display_error_message(
		    "No available window pattern card exists for this cofniguration!");
	}

	change_page_number(0);
	hbox_Pager.show();
}

void BoardConfigSubpage::create_wpc(const wpc_c& creatableWPC)
{
	SizeInPixels wpcSize{size_t(240 * 1.1), 240};
	size_t leftAndRightAlign = (ctx.size.width - (wpcChoicePerPage * (wpcSize.width + 10))) / 2;
	size_t ind               = 0;
	for (auto&& wpc : creatableWPC) {
		selectableWPCBySelectedConfig.push_back(SelectableWPCConfig{240, wpc});

		auto& lastlyAddedTestView =
		    selectableWPCBySelectedConfig[selectableWPCBySelectedConfig.size() - 1];

		lastlyAddedTestView.hide();

		put(lastlyAddedTestView, leftAndRightAlign + ind * (wpcSize.width + 10),
		    ctx.size.height - 105 - wpcSize.height);

		ind = (ind + 1) % wpcChoicePerPage;
	}
}

void BoardConfigSubpage::display_error_message(const std::string& text)
{
	hbox_Pager.hide();
	ctx.mainWindow.display_error_message("Invalid configuration", text);
}

board_config_subpage_info_t BoardConfigSubpage::collect_information()
{
	auto selectedFilter =
	    std::views::filter([](const SelectableWPCConfig& obj) { return obj.is_selected(); });
	auto wpcConfigToCtx =
	    std::views::transform([](const SelectableWPCConfig& obj) { return obj.wpc; });

	// If the show config button was not pressed yet, the WPC are not
	// constructed
	if (selectableWPCBySelectedConfig.empty()) {
		show_config_button_clicked();
	}

	board_config_subpage_info_t enteredData = std::make_unique<BoardConfigSubpageInformation>();

	std::ranges::copy(selectableWPCBySelectedConfig | selectedFilter | wpcConfigToCtx,
	                  std::back_inserter(enteredData->selectableWpc));
	enteredData->boardConfig = get_board_config();

	return enteredData;
}