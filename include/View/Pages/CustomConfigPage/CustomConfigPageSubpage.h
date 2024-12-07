#ifndef ADVANCED_SETTINGS_PAGE_SUBPAGE_H
#define ADVANCED_SETTINGS_PAGE_SUBPAGE_H

#include <gtkmm.h>

#include "DefaultGameConstants.h"
#include "DefaultGameCreator.h"
#include "InformationNotProvidedException.h"
#include "Typedefs.h"
#include "ViewContext.h"

class CustomConfigPageSubpage : public Gtk::Fixed
{
  public:
    CustomConfigPageSubpage(const ViewContext& _ctx) : ctx(_ctx) {}

  protected:
    ViewContext ctx;
};

#endif // ADVANCED_SETTINGS_PAGE_SUBPAGE_H