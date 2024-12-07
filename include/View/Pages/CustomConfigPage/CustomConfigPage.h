#ifndef CUSTOM_CONFIG_PAGE_H
#define CUSTOM_CONFIG_PAGE_H

#include <memory>

#include "HomePage.h"
#include "Page.h"

struct AllSubpages;

class CustomConfigPage : public Page
{
  public:
    CustomConfigPage(const ViewContext& ctx);
    ~CustomConfigPage();

  private:
    Gtk::Fixed fixed;
    Gtk::Label tryLabel;

    Gtk::Notebook notebook_SettingsByParts;

    Gtk::HBox hbox_SaveAndQuitButtons;
    Gtk::Button button_Save;
    Gtk::Button button_Quit;

    std::unique_ptr<AllSubpages> allSubpages;

    void save_button_clicked();
    void quit_button_clicked();
};

#endif // CUSTOM_CONFIG_PAGE_H