#ifndef AI_TYPE_SPECIFIER_VIEW_H
#define AI_TYPE_SPECIFIER_VIEW_H

#include "AI_PlayerConfig.h"
#include "Typedefs.h"

#include <gtkmm.h>

class AI_TypeSpecifierView : public Gtk::VBox {
  public:
	virtual ai_player_config_t create_player() = 0;
};

#endif // AI_TYPE_SPECIFIER_VIEW_H