#ifndef CHANGEDIEVALUETCVIEW_HPP
#define CHANGEDIEVALUETCVIEW_HPP

#include "Attributes.h"
#include "BoardFieldView.h"
#include "BoardIndex.h"
#include "ChangeDieValueTC.h"
#include "Die.h"
#include "ToolCardView.h"
#include "Typedefs.h"

#include <memory>

class ChangeDieValueTCView : public ToolCardView {
  public:
	ChangeDieValueTCView(ToolCardViewContainer& _parentContainer, Game& game, ToolCard& _tc,
	                     const SizeInPixels& size, size_t ind)
	    : ToolCardView(_parentContainer, game, _tc, size, ind), tc((ChangeDieValueTC&)(_tc)),
	      selectedDie(nullptr), selectedValue(0),
	      plusOneValueContext(Point{10, 10}, SizeInPixels{50, 50}, 1),
	      minusOneValueContext(Point{63, 10}, SizeInPixels{50, 50}, -1)
	{
	}

	virtual void on_common_help_visualizer_draw(const Cairo::RefPtr<Cairo::Context>& cr) override
	{
		if (!selectedDie) {
			return;
		}

		visualize_context(cr, plusOneValueContext);
		visualize_context(cr, minusOneValueContext);
	}

	virtual void on_common_help_visualizer_click(GdkEventButton* event) override
	{
		if (!selectedDie) {
			return;
		}

		Die* tmpDie = selectedDie;
		reset();
		selectedDie = tmpDie;

		plusOneValueContext.fieldView->set_selected(nullptr);
		minusOneValueContext.fieldView->set_selected(nullptr);

		bool clickHandled = check_click_for_context(event, plusOneValueContext);
		if (!clickHandled) {
			check_click_for_context(event, minusOneValueContext);
		}

		parentContainer.queue_draw_common_area();
	}

	virtual void receive_board_field_click_signal(RectangularBoardView*,
	                                              const BoardIndex& boardInd) override
	{
		if (!selectedDie || selectedValue == 0) {
			return signalProcessor->show_error_message(
			    "You have to select a die and modify its value by clicking one "
			    "of the options "
			    "above!");
		}

		ID_t playerOnMoveID = game.get_curr_state()->player_on_move_id();
		auto move =
		    new ChangeDieValueTC::Move(playerOnMoveID, ind, selectedDie, selectedValue, boardInd);

		// Before sending the move, it has to be reset, because the Tool card's
		// use method expects the original value of the die
		reset();
		signalProcessor->send_move_request(move, moveResultHandlingInfo);
	}

	virtual void receive_die_click_signal(Die* die) override
	{
		reset();

		fill_cache_for_die(die);
		if (plusOneValueContext.cachedValueIndices.empty() &&
		    minusOneValueContext.cachedValueIndices.empty()) {
			return signalProcessor->show_error_message(
			    "None of the modification would make the die placeable!");
		}

		selectedDie = die;
		parentContainer.queue_draw_common_area();
	}

	virtual void reset() override
	{
		if (selectedDie) {
			tc.modify_die_value(selectedDie, -1 * selectedValue);
		}
		selectedDie   = nullptr;
		selectedValue = 0;
		plusOneValueContext.fieldView->set_selected(nullptr);
		minusOneValueContext.fieldView->set_selected(nullptr);
	}

  private:
	struct ValueBasedContext {
		ValueBasedContext(const Point& upperLeft, const SizeInPixels& fieldSize, int _value)
		    : underlyingDie(color_t::RED, value_t::ONE), value(_value)
		{
			underlyingField = BoardField::create_empty();
			underlyingField->put_die(&underlyingDie);
			fieldView = std::make_unique<BoardFieldView>(upperLeft, fieldSize, *underlyingField);
		}

		board_ind_c                     cachedValueIndices;
		board_field_t                   underlyingField;
		Die                             underlyingDie;
		std::unique_ptr<BoardFieldView> fieldView;
		int                             value;
	};

	ChangeDieValueTC& tc;
	Die*              selectedDie;
	int               selectedValue;

	ValueBasedContext plusOneValueContext;
	ValueBasedContext minusOneValueContext;

	constexpr static const MoveResultHandlingPolicy moveResultHandlingInfo{
	    .deselectOnFailure = true, .deselectOnSuccess = true};

	void visualize_context(const Cairo::RefPtr<Cairo::Context>& cr, ValueBasedContext& valueContext)
	{
		if (valueContext.cachedValueIndices.empty()) {
			return;
		}

		if (selectedValue == 0) {
			value_t newValue = value_t(size_t(selectedDie->get_value()) + valueContext.value);
			valueContext.underlyingDie.change_value(newValue);
			valueContext.underlyingDie.change_color(selectedDie->get_color());
		}

		valueContext.fieldView->draw(cr, false);
	}

	bool check_click_for_context(GdkEventButton* event, ValueBasedContext& valueContext)
	{
		if (valueContext.cachedValueIndices.empty()) {
			return false;
		}

		if (!valueContext.fieldView->contains_point(Point{size_t(event->x), size_t(event->y)})) {
			return false;
		}

		valueContext.fieldView->set_selected(&orange);
		selectedValue = valueContext.value;
		tc.modify_die_value(selectedDie, valueContext.value);
		signalProcessor->highlight_fields(valueContext.cachedValueIndices,
		                                  game.get_curr_state()->player_on_move_id());
		signalProcessor->redraw_curr_rounds_dice();
		return true;
	}

	void fill_cache_for_die(Die* die)
	{
		move_c                tmpMoveContainer;
		ToolCardAppendContext tcAppendCtx{tmpMoveContainer, game, ind,
		                                  game.get_curr_state()->player_on_move_id(), nullptr};

		tc.append_possible_moves(tcAppendCtx);

		plusOneValueContext.cachedValueIndices.clear();
		minusOneValueContext.cachedValueIndices.clear();

		for (auto&& move : tmpMoveContainer) {
			ChangeDieValueTC::Move* castedMove = (ChangeDieValueTC::Move*)(move);

			if (castedMove->dtfm.die == die) {
				if (castedMove->changeVal == 1) {
					plusOneValueContext.cachedValueIndices.push_back(castedMove->dtfm.boardIndices);
				}
				else {
					minusOneValueContext.cachedValueIndices.push_back(
					    castedMove->dtfm.boardIndices);
				}
			}
		}
	}
};

#endif // CHANGEDIEVALUETCVIEW_HPP