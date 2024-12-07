#ifndef AI_TYPE_SPECIFIER_VIEW_H
#define AI_TYPE_SPECIFIER_VIEW_H

#include <gtkmm.h>

#include "AI_PlayerConfig.h"
#include "Typedefs.h"

class AI_TypeSpecifierView : public Gtk::VBox
{
  public:
    virtual ai_player_config_t create_player() = 0;
};

#endif // AI_TYPE_SPECIFIER_VIEW_H