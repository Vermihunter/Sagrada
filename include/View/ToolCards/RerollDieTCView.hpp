#ifndef REROLLDIETCVIEW_HPP
#define REROLLDIETCVIEW_HPP

#include "RerollDieTC.h"
#include "ToolCardView.h"

class RerollDieTCView : public ToolCardView
{
  public:
    RerollDieTCView(ToolCardViewContainer& _parentContainer, Game& game,
                    ToolCard& _tc, const SizeInPixels& size, size_t ind)
        : ToolCardView(_parentContainer, game, _tc, size, ind)
        , tc((RerollDieTC&)(_tc))
        , selectedDie(nullptr)
    {
    }

    virtual void receive_die_click_signal(Die* die)
    {
        if (selectedDie) {
            return;
        }

        selectedDie = die;

        ID_t playerOnMoveId = game.get_curr_state()->player_on_move_id();
        auto move = new RerollDieTC::Move(playerOnMoveId, ind, selectedDie,
                                          selectedDie->get_value());
        auto clonedMove =
            move->clone(*game.get_ctx()->supply, *game.get_ctx()->supply);
        const bool moveSuccessful = signalProcessor->send_move_request(
            move, rerollMoveResultHandlingInfo);

        // If the move was not successful, we don't have to care about the
        // following moves
        if (!moveSuccessful) {
            delete clonedMove;
            return;
        }

        // Collecting correct placeable indices
        move_c tmpMoveContainer;
        ToolCardAppendContext tmpCtx {tmpMoveContainer, game, ind,
                                      playerOnMoveId,
                                      tool_card_move_t(clonedMove)};
        tc.append_possible_moves(tmpCtx);

        if (tmpMoveContainer.empty()) {
            signalProcessor->show_error_message(
                "Unfortunately you cannot place the rerolled die!");
            // We are forcing the select because after there is no placing move
            // with the rerolled value of the new die so the back-end shifted
            // the rounds and if the next player on move is not the same one,
            // the tool_card_clicked() function would reject the click
            signalProcessor->tool_card_clicked(this, /* forceSelect =*/true);
            return;
        }

        board_ind_c selectableIndices;
        std::transform(tmpMoveContainer.begin(), tmpMoveContainer.end(),
                       std::back_inserter(selectableIndices),
                       [](const move_t& m) {
                           return ((PlacingDieMove*)(m))->bm.boardIndices;
                       });

        // highlight possible moves
        signalProcessor->highlight_fields(selectableIndices, playerOnMoveId);
        // Block everything except for placing the rerolled die
        signalProcessor->block_features(diePlacingBlockState);

        delete clonedMove;
    }

    void receive_board_field_click_signal(RectangularBoardView*,
                                          const BoardIndex& boardInd)
    {
        if (!selectedDie) {
            return signalProcessor->show_error_message(
                "You have to reroll a die before placing it!");
        }

        ID_t playerOnMoveId = game.get_curr_state()->player_on_move_id();
        auto placingMove =
            new PlacingDieMove(playerOnMoveId, ind,
                               DieToFieldMove {playerOnMoveId, selectedDie,
                                               boardInd, DTFM_MODE_DEFAULT});
        const bool moveSuccessful = signalProcessor->send_move_request(
            placingMove, placingMoveResultHandlingInfo);

        if (moveSuccessful) {
            signalProcessor->restore_default_blocking_state();
        }
    }

    // This move is not reversible
    virtual void reset() override { selectedDie = nullptr; }

  private:
    RerollDieTC& tc;
    Die* selectedDie;

    constexpr static const BlockedUserInterraction diePlacingBlockState {
        .blockedTcClick = true,
        .blockedRoundTrackClick = true,
        .blockedDieClick = true,
        .blockedBoardClick = false,
        .blockedPassMove = true};

    constexpr static const MoveResultHandlingPolicy
        placingMoveResultHandlingInfo {.deselectOnFailure = false,
                                       .deselectOnSuccess = true};

    constexpr static const MoveResultHandlingPolicy
        rerollMoveResultHandlingInfo {.deselectOnFailure = true,
                                      .deselectOnSuccess = false};
};

#endif // REROLLDIETCVIEW_HPP