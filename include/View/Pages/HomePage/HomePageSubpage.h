#ifndef HOME_PAGE_SUB_PAGE_H
#define HOME_PAGE_SUB_PAGE_H

#include "Page.h"

#include <gtkmm.h>

class HomePageSubPage : public Gtk::Fixed {
  public:
	HomePageSubPage(const ViewContext& _ctx, Gtk::Fixed& _prevPageWidgetContainer,
	                Gtk::Fixed& _homePageBackgroundFixed)
	    : ctx(_ctx), prevPageWidgetContainer(_prevPageWidgetContainer),
	      homePageBackgroundFixed(_homePageBackgroundFixed), button_Back("Back")
	{
		button_Back.signal_clicked().connect(
		    sigc::mem_fun(*this, &HomePageSubPage::back_button_clicked));
	}

	auto& get_ctx() { return ctx; }

  protected:
	ViewContext ctx;
	Gtk::Fixed& prevPageWidgetContainer;
	Gtk::Fixed& homePageBackgroundFixed;

	Gtk::Button button_Back;

  private:
	void back_button_clicked()
	{
		hide();
		prevPageWidgetContainer.show();
	}
};

#endif // HOME_PAGE_SUB_PAGE_H