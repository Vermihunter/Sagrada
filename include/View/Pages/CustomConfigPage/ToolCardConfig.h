#ifndef TOOL_CARD_CONFIG_H
#define TOOL_CARD_CONFIG_H

#include "Image.h"
#include "LabeledWidget.h"
#include "NumberEntry.h"
#include "ToolCardType.h"

#include <gtkmm.h>

class ToolCardConfig : public Gtk::VBox {
  public:
	ToolCardConfig(Image& image, tc_etype _type) : type(_type), checkButton_Selected("Select")
	{
		checkButton_Selected.set_active(true);
		checkButton_Selected.set_halign(Gtk::ALIGN_CENTER);

		pack_start(image);
		pack_start(checkButton_Selected, Gtk::PACK_SHRINK, 5); //
		show_all_children();
	}

	bool is_selected() { return checkButton_Selected.get_active(); }

	const tc_etype type;

  private:
	Gtk::CheckButton checkButton_Selected;
};

#endif // TOOL_CARD_CONFIG_H