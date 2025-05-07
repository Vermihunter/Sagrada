#ifndef SELECTABLE_WPC_CONFIG_H
#define SELECTABLE_WPC_CONFIG_H

#include "RectangularBoardView.h"

#include <gtkmm.h>

struct SelectableWPCConfig : public Gtk::VBox {
	SelectableWPCConfig(size_t wpcWidth, wpc_t _wpc) : wpc(_wpc), checkButton_Selected("Select")
	{
		wpcView = std::make_unique<RectangularBoardView>(
		    *wpc,
		    SizeInPixels{wpcWidth, size_t(wpcWidth / rectangularBoardViewWidthToHeightRatio)});
		checkButton_Selected.set_active(true);
		checkButton_Selected.set_halign(Gtk::ALIGN_CENTER);

		pack_start(*wpcView);
		pack_start(checkButton_Selected);
		show_all_children();
	}

	bool is_selected() const { return checkButton_Selected.get_active(); }

	wpc_t                                 wpc;
	std::unique_ptr<RectangularBoardView> wpcView;
	Gtk::CheckButton                      checkButton_Selected;
};

#endif // SELECTABLE_WPC_CONFIG_H