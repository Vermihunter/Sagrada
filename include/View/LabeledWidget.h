#ifndef LABELED_WIDGET_H
#define LABELED_WIDGET_H

#include <gtkmm.h>

#include <concepts>
#include <string>

template <std::derived_from<Gtk::Widget> WidgetType, bool IsEntry = true>
class LabeledWidget : public Gtk::VBox
{
  public:
    LabeledWidget(const std::string& labelText,
                  const std::string& entryText = "")
        : label(labelText)
    {
        if constexpr (IsEntry) {
            widget.set_text(entryText);
        }

        pack_start(label);
        pack_start(widget);
    }

    Gtk::Label label;
    WidgetType widget;
};

#endif // LABELED_WIDGET_H