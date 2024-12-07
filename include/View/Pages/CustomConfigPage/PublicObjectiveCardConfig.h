#ifndef PUBLIC_OBJECTIVE_CARD_CONFIG_H
#define PUBLIC_OBJECTIVE_CARD_CONFIG_H

#include <gtkmm.h>

#include "Image.h"
#include "LabeledWidget.h"
#include "NumberEntry.h"
#include "PublicObjectiveCardContext.h"
#include "PublicObjectiveCardType.h"

class PublicObjectiveCardConfig : public Gtk::VBox
{
  public:
    PublicObjectiveCardConfig(Image& image, puoc_etype _type,
                              size_t defaultValue)
        : type(_type)
        , checkButton_Selected("Select")
        , lne_SatisfactionValue("Satisfaction value",
                                std::to_string(defaultValue))
    {
        checkButton_Selected.set_active(true);
        checkButton_Selected.set_halign(Gtk::ALIGN_CENTER);

        pack_start(image);
        pack_start(lne_SatisfactionValue, Gtk::PACK_SHRINK, 4);
        pack_start(checkButton_Selected, Gtk::PACK_SHRINK, 4);

        show_all_children();
    }

    bool is_selected() { return checkButton_Selected.get_active(); }

    const puoc_etype type;
    Gtk::CheckButton checkButton_Selected;
    LabeledWidget<NumberEntry> lne_SatisfactionValue;
};

#endif // PUBLIC_OBJECTIVE_CARD_CONFIG_H