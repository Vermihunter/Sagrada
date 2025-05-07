#ifndef REPLACEDIEFROMSUPPLYTOROUNDTRACKTCVIEW_HPP
#define REPLACEDIEFROMSUPPLYTOROUNDTRACKTCVIEW_HPP

#include "ReplaceDieFromSupplyToRoundTrackTC.h"
#include "ToolCardView.h"

class ReplaceDieFromSupplyToRoundTrackTCView : public ToolCardView {
  public:
	ReplaceDieFromSupplyToRoundTrackTCView(ToolCardViewContainer& _parentContainer, Game& game,
	                                       ToolCard& tc, const SizeInPixels& size, size_t ind)
	    : ToolCardView(_parentContainer, game, tc, size, ind), currSupplyDie(nullptr),
	      currRoundTrackDie(nullptr)
	{
	}

	virtual void receive_die_click_signal(Die* supplyDie) override
	{
		std::cout << "Saved as supplyDie: " << supplyDie << std::endl;
		reset();

		currSupplyDie     = supplyDie;
		currRoundTrackDie = nullptr;
	}

	virtual void receive_round_track_die_click_signal(Die* roundTrackDie) override
	{
		if (!currSupplyDie) {
			return signalProcessor->show_error_message(
			    "You have to select a die first from the supply to replace "
			    "with!");
		}

		std::cout << "Saved as round track: " << roundTrackDie << std::endl;

		Die* tmpCurrSupplyDie = currSupplyDie;
		reset();
		currSupplyDie = tmpCurrSupplyDie;

		ID_t        playerOnMoveId   = game.get_curr_state()->player_on_move_id();
		board_ind_c placeableIndices = game.get_ctx()->basicMoveHandler->indices_to_put_to(
		    game, playerOnMoveId, roundTrackDie);

		if (placeableIndices.empty()) {
			return signalProcessor->show_error_message(
			    "The round track die would not be placeable!");
		}

		currRoundTrackDie = roundTrackDie;
		std::swap(*currSupplyDie, *currRoundTrackDie);

		signalProcessor->redraw_curr_rounds_dice();
		signalProcessor->block_features(diePlacingBlockState);
		signalProcessor->highlight_fields(placeableIndices, playerOnMoveId);
	}

	virtual void receive_board_field_click_signal(RectangularBoardView*, const BoardIndex& boardInd)
	{
		if (!currSupplyDie || !currRoundTrackDie) {
			return signalProcessor->show_error_message(
			    "Select a die from the supply and a die from the round track "
			    "before trying to "
			    "place it!");
		}

		ID_t playerOnMoveId = game.get_curr_state()->player_on_move_id();
		std::swap(*currSupplyDie, *currRoundTrackDie);
		// Has to create pointer because the destructor of
		// ReplaceDieFromSupplyToRoundTrackTC::Move delete it
		DieToFieldMove dtfm{playerOnMoveId, currSupplyDie, boardInd, DTFM_MODE_DEFAULT};
		auto move = new ReplaceDieFromSupplyToRoundTrackTC::Move(playerOnMoveId, ind, currSupplyDie,
		                                                         currRoundTrackDie, dtfm);

		// Has to clear them because if the move is correct, reset() is called
		// and it would swap the dice
		currSupplyDie                 = nullptr;
		currRoundTrackDie             = nullptr;
		auto       tmpDieInSupply     = move->dieInSupply;
		auto       tmpDieOnRoundTrack = move->dieOnRoundTrack;
		const bool moveSuccessful =
		    signalProcessor->send_move_request(move, moveResultHandlingInfo);

		if (moveSuccessful) {
			signalProcessor->restore_default_blocking_state();
		}
		else {
			currSupplyDie     = tmpDieInSupply;
			currRoundTrackDie = tmpDieOnRoundTrack;
			reset();
		}
	}

	virtual void reset() override
	{
		if (currSupplyDie && currRoundTrackDie) {
			std::swap(*currSupplyDie, *currRoundTrackDie);
		}

		currSupplyDie     = nullptr;
		currRoundTrackDie = nullptr;

		signalProcessor->restore_default_blocking_state();
	}

  private:
	Die* currSupplyDie;
	Die* currRoundTrackDie;

	constexpr static const BlockedUserInterraction diePlacingBlockState{.blockedTcClick = true,
	                                                                    .blockedRoundTrackClick =
	                                                                        true,
	                                                                    .blockedDieClick   = true,
	                                                                    .blockedBoardClick = false,
	                                                                    .blockedPassMove   = true};

	constexpr static const MoveResultHandlingPolicy moveResultHandlingInfo{
	    .deselectOnFailure = true, .deselectOnSuccess = true};
};

#endif // REPLACEDIEFROMSUPPLYTOROUNDTRACKTCVIEW_HPP