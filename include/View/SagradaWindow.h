#ifndef SAGRADA_WINDOW_H
#define SAGRADA_WINDOW_H

#include <gtkmm.h>

#include "CustomConfigPage.h"
#include "GameMode.h"
#include "HomePage.h"
#include "Image.h"
#include "PageType.h"

class SagradaWindow : public Gtk::Window
{
  public:
    SagradaWindow(const SizeInPixels& size,
                  game_mode_t gameMode = game_mode_t::NONE,
                  tournament_config_t tournamentConfig = nullptr);

    void display_error_message(const std::string& title,
                               const std::string& text = "")
    {
        Gtk::MessageDialog dialog {*this, title};
        dialog.set_secondary_text(text);
        dialog.run();
    }

    void show_page(Page& page, bool withSagradaLogo = false);
    void show_home_page();
    void show_custom_config_page();
    void add_page(Page& newPage, const Point& pos = Point {0, 0});
    void remove_page(Page& page);

    HomePage homePage;

  private:
    Page* currPage;
    Image sagradaLogo;
    Gtk::Fixed fixed_Logo;
    Gtk::Fixed fixed;
    SizeInPixels size;

    std::unique_ptr<CustomConfigPage> customConfigPage;
};

#endif // SAGRADA_WINDOW_H