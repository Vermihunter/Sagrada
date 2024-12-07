#ifndef CARD_ICON_H
#define CARD_ICON_H

#include <gtkmm.h>

#include <memory>

#include "Image.h"
#include "SizeInPixels.h"
#include "ViewConstants.h"

class CardIcon : public Gtk::EventBox
{
  public:
    CardIcon(const std::string& iconPath, const SizeInPixels& size,
             Gtk::Box& _cards, const std::string& tooltipText,
             Gtk::Box*& _currentlyShownCards)
        : cards(_cards)
        , currentlyShownCards(_currentlyShownCards)
        , icon(iconPath, size)
    {
        add(icon);
        set_tooltip_text(tooltipText);
        add_events(Gdk::BUTTON_PRESS_MASK);
        cards.hide();
    }

    static auto create_puoc_icon(const SizeInPixels& size, Gtk::HBox& cards,
                                 Gtk::Box*& currentlyShownCards)
    {
        return std::make_unique<CardIcon>(v_constants::puocIconPath, size,
                                          cards, "Public objective cards",
                                          currentlyShownCards);
    }

    static auto create_tc_icon(const SizeInPixels& size, Gtk::VBox& cards,
                               Gtk::Box*& currentlyShownCards)
    {
        return std::make_unique<CardIcon>(v_constants::tcIconPath, size, cards,
                                          "Tool cards", currentlyShownCards);
    }

    static auto create_proc_icon(const SizeInPixels& size, Gtk::HBox& card,
                                 Gtk::Box*& currentlyShownCards)
    {
        return std::make_unique<CardIcon>(v_constants::procIconPath, size, card,
                                          "Private objective card",
                                          currentlyShownCards);
    }

    void show() { cards.show(); }

  protected:
    virtual bool on_button_press_event(GdkEventButton* event)
    {
        if (currentlyShownCards == &cards) {
            currentlyShownCards = nullptr;
            cards.hide();
            return true;
        }

        if (currentlyShownCards) {
            currentlyShownCards->hide();
        }

        currentlyShownCards = &cards;
        currentlyShownCards->show();

        return true;
    }

  private:
    Gtk::Box& cards;
    Gtk::Box*& currentlyShownCards;
    Image icon;
};

using card_icon_t = std::unique_ptr<CardIcon>;

#endif // CARD_ICON_H
