#include "ToolCardViewContainer.h"

#include "ToolCardView.h"

ToolCardViewContainer::ToolCardViewContainer(Game& game, size_t cardWidth)
    : currToolCardView(nullptr)
{
	size_t ind = 0;
	for (auto&& gameTc : game.get_ctx()->toolCards) {
		tcCards.push_back(ToolCardView::create(
		    *this, game, *gameTc, SizeInPixels{cardWidth, size_t(cardWidth / tcWidthToHeightRatio)},
		    ind));
		tcViewBox.pack_start(*tcCards.back(), Gtk::PACK_EXPAND_PADDING, 10);
		++ind;
	}

	pack_start(tcViewBox);
	show_all_children();
}

void ToolCardViewContainer::queue_draw_common_area()
{
	commonDrawingArea->queue_draw();
}

void ToolCardViewContainer::construct_common_drawing_area()
{
	commonDrawingArea = std::make_unique<Gtk::DrawingArea>();
	commonDrawingArea->signal_draw().connect(
	    sigc::mem_fun(*this, &ToolCardViewContainer::on_common_da_draw));

	commonDrawingArea->signal_button_press_event().connect(
	    sigc::mem_fun(*this, &ToolCardViewContainer::on_common_da_click));

	commonDrawingArea->add_events(Gdk::BUTTON_PRESS_MASK);

	commonDrawingArea->set_size_request(-1, 100);
	pack_start(*commonDrawingArea);
	show_all_children();
}

void ToolCardViewContainer::connect_tool_card(ToolCardView* newlySelectedToolCardView)
{
	if (newlySelectedToolCardView == nullptr) {
		commonDrawingArea->hide();
	}
	else {
		commonDrawingArea->show();
		commonDrawingArea->queue_draw();
	}

	currToolCardView = newlySelectedToolCardView;
}

bool ToolCardViewContainer::on_common_da_click(GdkEventButton* event)
{
	if (currToolCardView) {
		currToolCardView->on_common_help_visualizer_click(event);
	}

	return true;
}

bool ToolCardViewContainer::on_common_da_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	if (currToolCardView) {
		currToolCardView->on_common_help_visualizer_draw(cr);
	}

	return true;
}