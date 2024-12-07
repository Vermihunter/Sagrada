#ifndef TOOL_CARD_VIEW_CONTAINER_H
#define TOOL_CARD_VIEW_CONTAINER_H

#include <gtkmm.h>

#include <memory>

#include "Game.h"
#include "ToolCard.h"
#include "ToolCardViewConstants.h"
#include "Typedefs.h"

class ToolCardView;

class ToolCardViewContainer : public Gtk::VBox
{
  public:
    ToolCardViewContainer(Game& game, size_t cardWidth);

    void construct_common_drawing_area();
    void connect_tool_card(ToolCardView*);

    /** Tool cards */
    Gtk::HBox tcViewBox;
    tc_view_c tcCards;

    void queue_draw_common_area();

  private:
    ToolCardView* currToolCardView;
    std::unique_ptr<Gtk::DrawingArea> commonDrawingArea;

    bool on_common_da_draw(const Cairo::RefPtr<Cairo::Context>& cr);
    bool on_common_da_click(GdkEventButton* event);
};

#endif // TOOL_CARD_VIEW_CONTAINER_H