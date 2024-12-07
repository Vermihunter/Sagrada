#ifndef PAGE_H
#define PAGE_H

#include <gtkmm.h>

#include <vector>

#include "ViewContext.h"

class Page : public Gtk::Frame
{
  public:
    void hide_widgets()
    {
        this->hide();
        for (auto&& widget : widgets)
            widget->hide();
    }

    void show_widgets()
    {
        for (auto&& widget : widgets)
            widget->show();
    }

  protected:
    Page(const ViewContext& _ctx) : ctx(_ctx)
    {
        set_size_request(ctx.size.width, ctx.size.height);
    }

    ViewContext ctx;
    std::vector<Gtk::Widget*> widgets;
};

#endif // PAGE_H