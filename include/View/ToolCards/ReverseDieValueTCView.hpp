#ifndef REVERSEDIEVALUETCVIEW_HPP
#define REVERSEDIEVALUETCVIEW_HPP

#include "ReverseDieValueTC.h"
#include "ToolCardView.h"

class ReverseDieValueTCView : public ToolCardView {
  public:
	ReverseDieValueTCView(ToolCardViewContainer& _parentContainer, Game& game, ToolCard& tc,
	                      const SizeInPixels& size, size_t ind)
	    : ToolCardView(_parentContainer, game, tc, size, ind), selectedDie(nullptr)
	{
	}

	virtual void receive_die_click_signal(Die* die) override
	{
		// Re-reversing previously selected die
		if (selectedDie) {
			ReverseDieValueTC::reverse_die_value(selectedDie);
		}

		selectedDie         = die;
		ID_t playerOnMoveId = game.get_curr_state()->player_on_move_id();

		// Visualizing the reversion so the player saw the new die
		ReverseDieValueTC::reverse_die_value(selectedDie);

		board_ind_c reversedDieValuePlaceableIndices =
		    game.get_ctx()->basicMoveHandler->indices_to_put_to(game, playerOnMoveId, selectedDie);
		if (reversedDieValuePlaceableIndices.empty()) {
			ReverseDieValueTC::reverse_die_value(selectedDie);
			return signalProcessor->show_error_message(
			    "The reversed die would not be placeable on to the board!");
		}

		signalProcessor->redraw_curr_rounds_dice();
		signalProcessor->highlight_fields(reversedDieValuePlaceableIndices, playerOnMoveId);
	}

	virtual void receive_board_field_click_signal(RectangularBoardView* boardView,
	                                              const BoardIndex&     boardInd)
	{
		if (!selectedDie) {
			return signalProcessor->show_error_message(
			    "You have to select a die before placing it!");
		}

		Die* tmpDie   = selectedDie;
		ID_t playerID = game.get_curr_state()->player_on_move_id();
		reset();
		auto move = new PlacingDieMove(
		    playerID, ind, DieToFieldMove{playerID, tmpDie, boardInd, DTFM_MODE_DEFAULT});
		signalProcessor->send_move_request(move, moveResultHandlingInfo);
	}

	virtual void reset() override
	{
		if (selectedDie) {
			ReverseDieValueTC::reverse_die_value(selectedDie);
			signalProcessor->redraw_curr_rounds_dice();
		}

		selectedDie = nullptr;
	}

  private:
	Die* selectedDie;

	constexpr static const MoveResultHandlingPolicy moveResultHandlingInfo{
	    .deselectOnFailure = true, .deselectOnSuccess = true};
};

#endif // REVERSEDIEVALUETCVIEW_HPP