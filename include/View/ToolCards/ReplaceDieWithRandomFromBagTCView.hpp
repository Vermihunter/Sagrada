#ifndef REPLACEDIEWITHRANDOMFROMBAGTCVIEW_HPP
#define REPLACEDIEWITHRANDOMFROMBAGTCVIEW_HPP

#include "Attributes.h"
#include "ReplaceDieWithRandomFromBagTC.h"
#include "ToolCardView.h"

#include <array>

class ReplaceDieWithRandomFromBagTCView : public ToolCardView {
  public:
	ReplaceDieWithRandomFromBagTCView(ToolCardViewContainer& _parentContainer, Game& game,
	                                  ToolCard& _tc, const SizeInPixels& size, size_t ind)
	    : ToolCardView(_parentContainer, game, _tc, size, ind),
	      tc((ReplaceDieWithRandomFromBagTC&)(_tc))
	{
		for (size_t i = 0; i < size_t(value_t::COUNT); ++i) {
			valueContexts[i] = std::make_unique<ValueContext>(value_t(i), i);
		}
	}

	virtual void on_common_help_visualizer_draw(const Cairo::RefPtr<Cairo::Context>& cr) override
	{
		if (!selectedDie) {
			return;
		}

		for (auto&& valueContext : valueContexts) {
			if (valueContext->usableIndices.empty()) {
				continue;
			}

			valueContext->boardFieldDie.change_color(selectedDie->get_color());
			valueContext->boardFieldView->draw(cr, false);
		}
	}

	virtual void on_common_help_visualizer_click(GdkEventButton* event) override
	{
		if (!selectedDie) {
			return;
		}

		Point clickPoint{size_t(event->x), size_t(event->y)};
		for (auto&& valueContext : valueContexts) {
			if (valueContext->boardFieldView->contains_point(clickPoint)) {
				valueContext->boardFieldView->set_selected(&orange);
				selectedDie->change_value(valueContext->value);
				signalProcessor->highlight_fields(valueContext->usableIndices,
				                                  game.get_curr_state()->player_on_move_id());
			}
			else {
				valueContext->boardFieldView->set_selected(nullptr);
			}
		}

		signalProcessor->redraw_curr_rounds_dice();
		parentContainer.queue_draw_common_area();
	}

	virtual void receive_board_field_click_signal(RectangularBoardView*,
	                                              const BoardIndex& boardInd) override
	{
		if (!selectedDie) {
			return signalProcessor->show_error_message(
			    "Select a die to change with a random die from the bag!");
		}

		ID_t playerOnMoveID = game.get_curr_state()->player_on_move_id();
		ReplaceDieWithRandomFromBagTC::ValueChangerAndPlacerMove move{
		    playerOnMoveID,           ind,         selectedDie->get_value(),
		    selectedDieOriginalValue, selectedDie, boardInd};

		const bool moveSuccessful =
		    signalProcessor->send_move_request(&move, diePlacingMoveResultHandlingInfo);

		if (moveSuccessful) {
			signalProcessor->restore_default_blocking_state();
		}
	}

	virtual void receive_die_click_signal(Die* die) override
	{
		if (selectedDie) {
			return;
		}

		selectedDie = die;

		ID_t playerOnMoveID = game.get_curr_state()->player_on_move_id();
		auto move =
		    new ReplaceDieWithRandomFromBagTC::DieToChangeMove(playerOnMoveID, ind, selectedDie);
		bool       existsMove = fill_cache_for_move(move);
		const bool moveSuccessful =
		    signalProcessor->send_move_request(move, bagPlaceMoveResultHandlingInfo);

		// Here the dice are already swapped, so the pointer points to the
		// swapped die
		selectedDieOriginalValue = selectedDie->get_value();

		if (!moveSuccessful) {
			return;
		}

		if (!existsMove) {
			signalProcessor->show_error_message(
			    "Unfortunately you cannot place the die from the bag with any "
			    "values!");
			// The tool card click calls reset, so one more explicit call is not
			// necessary
			signalProcessor->tool_card_clicked(this, /* forceSelect =*/true);
			return;
		}

		signalProcessor->block_features(diePlacingBlockState);
		parentContainer.queue_draw_common_area();
	}

	virtual void reset() override
	{
		selectedDie = nullptr;
		reset_all_value_contexts();
	}

  private:
	struct ValueContext {
		ValueContext(value_t _val, size_t ind) : boardFieldDie(color_t::RED, _val)
		{
			value = _val;
			// boardField = BoardField::create_empty();
			boardField.put_die(&boardFieldDie);
			boardFieldView = std::make_unique<BoardFieldView>(Point{10 + ind * (50 + 7), 10},
			                                                  SizeInPixels{50, 50}, boardField);
		}

		void reset()
		{
			usableIndices.clear();
			boardFieldView->set_selected(nullptr);
		}

		value_t                         value;
		board_ind_c                     usableIndices;
		Die                             boardFieldDie;
		BoardField                      boardField;
		std::unique_ptr<BoardFieldView> boardFieldView;
	};

	ReplaceDieWithRandomFromBagTC& tc;
	Die*                           selectedDie;
	value_t                        selectedDieOriginalValue;

	std::array<std::unique_ptr<ValueContext>, Value::SIZE> valueContexts;

	constexpr static const MoveResultHandlingPolicy bagPlaceMoveResultHandlingInfo{
	    .deselectOnFailure = true, .deselectOnSuccess = false};

	constexpr static const MoveResultHandlingPolicy diePlacingMoveResultHandlingInfo{
	    .deselectOnFailure = false, .deselectOnSuccess = true};

	constexpr static const BlockedUserInterraction diePlacingBlockState{.blockedTcClick = true,
	                                                                    .blockedRoundTrackClick =
	                                                                        true,
	                                                                    .blockedDieClick   = true,
	                                                                    .blockedBoardClick = false,
	                                                                    .blockedPassMove   = true};

	void reset_all_value_contexts()
	{
		for (auto&& valueContext : valueContexts) {
			valueContext->reset();
		}
	}

	/**
	 * @brief
	 *
	 * @param prevMove The previous Supply-with-bag die changing move
	 * @return true If at least one move exists for at least one value
	 * @return false If the die from the bag has no value that could make it
	 * placeable to the board
	 */
	bool fill_cache_for_move(ReplaceDieWithRandomFromBagTC::DieToChangeMove* prevMove)
	{
		move_c                tmpMoveContainer;
		ToolCardAppendContext tmpAppendCtx{tmpMoveContainer, game, ind, prevMove->playerId,
		                                   prevMove};
		reset_all_value_contexts();

		tc.append_possible_moves(tmpAppendCtx);

		for (auto&& move : tmpMoveContainer) {
			auto castedMove = (ReplaceDieWithRandomFromBagTC::ValueChangerAndPlacerMove*)(move);
			valueContexts[size_t(castedMove->newVal)]->usableIndices.push_back(
			    castedMove->placingMove.boardIndices);
		}

		return std::any_of(valueContexts.begin(), valueContexts.end(),
		                   [](const std::unique_ptr<ValueContext>& valueContext) {
			                   return !valueContext->usableIndices.empty();
		                   });
	}
};

#endif // REPLACEDIEWITHRANDOMFROMBAGTCVIEW_HPP