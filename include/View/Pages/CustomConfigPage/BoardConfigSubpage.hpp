#ifndef BOARDCONFIGSUBPAGE_HPP
#define BOARDCONFIGSUBPAGE_HPP

#include "BoardConfigSubpageInformation.h"
#include "BoardType.h"
#include "CustomConfigPageSubpage.h"
#include "NumberEntry.h"
#include "RectangularBoardView.h"
#include "SelectableWPCConfig.h"

#include <ranges>

class BoardConfigSubpage : public CustomConfigPageSubpage {
  public:
	BoardConfigSubpage(const ViewContext& ctx);

	board_config_subpage_info_t collect_information();

  protected:
	virtual void on_show() override;

  private:
	/** Dimensions */
	Gtk::Label label_Dimensions;
	// Width
	Gtk::VBox   vbox_Width;
	Gtk::Label  label_Width;
	NumberEntry nEntry_Width;
	// Height
	Gtk::VBox   vbox_Height;
	Gtk::Label  label_Height;
	NumberEntry nEntry_Height;
	// Dimensions together
	Gtk::HBox hbox_Dimensions;

	/** Board type */
	Gtk::Label        label_BoardTypeChooser;
	Gtk::ComboBoxText cb_BoardTypeChooser;
	Gtk::VBox         vbox_BoardTypeChooser;

	Gtk::Button button_ConfigureBoardOptions;

	/** Config - All together */
	Gtk::VBox  vbox_Config;
	Gtk::Frame frame_Config;

	/** Pager buttons*/
	Gtk::HBox   hbox_Pager;
	Gtk::Button button_Next;
	Gtk::Button button_Prev;

	int          currPageNumber;
	BoardConfig* currBoardConfig;

	std::vector<SelectableWPCConfig> selectableWPCBySelectedConfig;

	static constexpr size_t wpcChoicePerPage = 4;

	void         change_page_number(int changeVal);
	void         create_wpc(const wpc_c& creatableWPC);
	void         show_config_button_clicked();
	void         display_error_message(const std::string& text);
	BoardConfig* get_board_config();
};

#endif // BOARDCONFIGSUBPAGE_HPP
