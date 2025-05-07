#include "ToolCardView.h"

#include "ChangeDieValueTCView.hpp"
#include "Move.h"
#include "MoveTwiceInARowTCView.hpp"
#include "PutDieWithoutNeighborTCView.hpp"
#include "RelocateDieIgnoreAttributeTCView.hpp"
#include "RelocateExactlyTwoDiceTCView.hpp"
#include "RelocateMaxTwoSameColorDiceTCView.hpp"
#include "ReplaceDieFromSupplyToRoundTrackTCView.hpp"
#include "ReplaceDieWithRandomFromBagTCView.hpp"
#include "RerollAllDiceTCView.hpp"
#include "RerollDieTCView.hpp"
#include "ReverseDieValueTCView.hpp"
#include "ToolCardViewContainer.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

ToolCardView::ToolCardView(ToolCardViewContainer& _parentContainer, Game& _game, ToolCard& _tc,
                           const SizeInPixels& size, size_t _ind)
    : parentContainer(_parentContainer), game(_game), tc(_tc), ind(_ind),
      selectionSize(2 * lineWidth + size.width, 2 * lineWidth + size.height),
      costSize(selectionSize.width, 22), image(tcImagePaths.at(_tc.get_type()), size),
      isSelected(false), signalProcessor(nullptr)
{
	da_SelectionArea.signal_draw().connect(sigc::mem_fun(*this, &ToolCardView::on_selection_draw));

	da_SelectionArea.set_size_request(selectionSize.width, selectionSize.height);

	eb_Image.add(image);
	eb_Image.add_events(Gdk::BUTTON_PRESS_MASK);
	eb_Image.signal_button_press_event().connect(
	    sigc::mem_fun(*this, &ToolCardView::on_tc_image_click));

	put(da_SelectionArea, 0, 0);
	put(eb_Image, lineWidth, lineWidth);
}

tc_view_t ToolCardView::create(ToolCardViewContainer& parentContainer, Game& game, ToolCard& tc,
                               const SizeInPixels& size, size_t ind)
{
	tc_view_t result;
	switch (tc.get_type()) {
	case tc_etype::CHANGE_DIE_VALUE:
		result = std::make_unique<ChangeDieValueTCView>(parentContainer, game, tc, size, ind);
		break;
	case tc_etype::MOVE_TWICE:
		result = std::make_unique<MoveTwiceInARowTCView>(parentContainer, game, tc, size, ind);
		break;
	case tc_etype::DIE_WITHOUT_NEIGHBORS:
		result =
		    std::make_unique<PutDieWithoutNeighborTCView>(parentContainer, game, tc, size, ind);
		break;
	case tc_etype::REROLL_ALL_DICE:
		result = std::make_unique<RerollAllDiceTCView>(parentContainer, game, tc, size, ind);
		break;
	case tc_etype::RELOCATE_TWO_SAME_COLORED_DICE:
		result = std::make_unique<RelocateMaxTwoSameColorDiceTCView>(parentContainer, game, tc,
		                                                             size, ind);
		break;
	case tc_etype::REPLACE_FROM_ROUND_TRACK:
		result = std::make_unique<ReplaceDieFromSupplyToRoundTrackTCView>(parentContainer, game, tc,
		                                                                  size, ind);
		break;
	case tc_etype::RANDOM_BAG_DIE:
		result = std::make_unique<ReplaceDieWithRandomFromBagTCView>(parentContainer, game, tc,
		                                                             size, ind);
		break;
	case tc_etype::REROLL_DIE:
		result = std::make_unique<RerollDieTCView>(parentContainer, game, tc, size, ind);
		break;
	case tc_etype::REVERSE_DIE_VALUE:
		result = std::make_unique<ReverseDieValueTCView>(parentContainer, game, tc, size, ind);
		break;
	case tc_etype::MOVE_IGNORE_VAL:
		result = std::make_unique<RelocateDieIgnoreAttributeTCView<DTFM_MODE_IGNORE_VALUE>>(
		    parentContainer, game, tc, size, ind);
		break;
	case tc_etype::MOVE_IGNORE_COLOR:
		result = std::make_unique<RelocateDieIgnoreAttributeTCView<DTFM_MODE_IGNORE_COLOR>>(
		    parentContainer, game, tc, size, ind);
		break;
	case tc_etype::RELOCATE_TWO_DICE:
		result =
		    std::make_unique<RelocateExactlyTwoDiceTCView>(parentContainer, game, tc, size, ind);
		break;
	case tc_etype::COUNT:
		break;
	default:
		break;
	}

	return result;
}

void ToolCardView::set_signal_receiver(IToolCardViewSignalProcessor* _signalProcessor)
{
	if (!signalProcessor && !da_CostVisualizer) {
		initialize_cost_visualizer();
	}

	signalProcessor = _signalProcessor;
}

/**
 * @brief Set the selected object
 * Marked  because if the tool card consists of a single move,
 * it won't be selected, but used instantly.
 *
 * @param _isSelected
 * @return true if the selection modification was successful
 * @return false if the selection cannot be modified
 */
bool ToolCardView::set_selected(bool _isSelected)
{
	if (!signalProcessor) {
		return false;
	}

	isSelected = _isSelected;
	queue_draw();
	return true;
}

/** Not forcing all the tool cards to implement all the possible signal handlers
 */
void ToolCardView::on_common_help_visualizer_draw(const Cairo::RefPtr<Cairo::Context>& cr) {}
void ToolCardView::on_common_help_visualizer_click(GdkEventButton* event) {}
void ToolCardView::receive_die_click_signal(Die*) {}
void ToolCardView::receive_round_track_die_click_signal(Die*) {}
void ToolCardView::reset() {}

bool ToolCardView::on_cost_visualizer_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	const static std::string costStr = "Cost:";

	/** Black "Cost" sign */
	cr->set_source_rgb(0.0, 0.0, 0.0);
	Cairo::TextExtents te;

	cr->select_font_face("Georgia", Cairo::FontSlant::FONT_SLANT_NORMAL,
	                     Cairo::FontWeight::FONT_WEIGHT_BOLD);
	cr->set_font_size(10);

	cr->get_text_extents(costStr, te);

	/** Center align */

	cr->move_to((selectionSize.width / 2) - te.x_bearing - te.width / 2,
	            costSize.height / 2 - te.y_bearing - te.height / 2);

	const SizeInPixels textEnd{size_t((selectionSize.width / 2) + te.x_bearing + te.width / 2),
	                           size_t(costSize.height / 2 + 1)};
	cr->show_text(costStr);

	const size_t circleRadius             = costSize.height / 6;
	const size_t x_distanceBetweenCircles = circleRadius + 3;
	for (size_t i = 0; i < tc.favor_tokens_to_use(); ++i) {
		cr->arc(textEnd.width + i * (circleRadius + x_distanceBetweenCircles) + 8, textEnd.height,
		        circleRadius, 0.0, 2 * M_PI);
	}
	cr->fill();

	return true;
}

move_c ToolCardView::collect_possible_moves(ToolCard& concreteToolCard, tool_card_move_t prevMove)
{
	move_c                tmpMoveContainer;
	ToolCardAppendContext tmpAppendContext{tmpMoveContainer, game, ind,
	                                       game.get_curr_state()->player_on_move_id(), prevMove};
	concreteToolCard.append_possible_moves(tmpAppendContext);
	return tmpMoveContainer;
}

bool ToolCardView::on_selection_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	if (!isSelected) {
		return true;
	}

	cr->set_source_rgb(1.0, 0.0, 0.0);
	cr->set_line_width(lineWidth);
	cr->rectangle(0, 0, selectionSize.width, selectionSize.height);
	cr->stroke();

	return true;
}

bool ToolCardView::on_tc_image_click(GdkEventButton* button_event)
{
	if (signalProcessor) {
		signalProcessor->tool_card_clicked(this);
	}

	return true;
}

void ToolCardView::initialize_cost_visualizer()
{
	da_CostVisualizer = std::make_unique<Gtk::DrawingArea>();
	da_CostVisualizer->signal_draw().connect(
	    sigc::mem_fun(*this, &ToolCardView::on_cost_visualizer_draw));

	da_CostVisualizer->set_size_request(costSize.width, costSize.height);
	put(*da_CostVisualizer, 0, selectionSize.height);
}