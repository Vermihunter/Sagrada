#ifndef PUTDIEWITHOUTNEIGHBORTCVIEW_HPP
#define PUTDIEWITHOUTNEIGHBORTCVIEW_HPP

#include "PutDieWithoutNeighborTC.h"
#include "ToolCardView.h"

class PutDieWithoutNeighborTCView : public ToolCardView
{
  public:
    PutDieWithoutNeighborTCView(ToolCardViewContainer& _parentContainer,
                                Game& game, ToolCard& _tc,
                                const SizeInPixels& size, size_t ind)
        : ToolCardView(_parentContainer, game, _tc, size, ind)
        , tc((PutDieWithoutNeighborTC&)(_tc))
        , selectedDie(nullptr)
    {
    }

    virtual void receive_die_click_signal(Die* die)
    {
        selectedDie = die;

        ID_t playerOnMoveId = game.get_curr_state()->player_on_move_id();

        // Collecting correct placeable indices
        move_c tmpMoveContainer;
        ToolCardAppendContext tmpCtx {tmpMoveContainer, game, ind,
                                      playerOnMoveId, nullptr};
        tc.append_possible_moves(tmpCtx);

        board_ind_c selectableBoardIndices;
        for (auto&& move : tmpMoveContainer) {
            PutDieWithoutNeighborTC::Move* castedMove =
                (PutDieWithoutNeighborTC::Move*)(move);
            if (castedMove->bm.die == selectedDie) {
                selectableBoardIndices.push_back(castedMove->bm.boardIndices);
            }

            delete move;
        }

        // highlight possible moves
        signalProcessor->highlight_fields(selectableBoardIndices,
                                          playerOnMoveId);
    }

    virtual void receive_board_field_click_signal(RectangularBoardView*,
                                                  const BoardIndex& boardInd)
    {
        if (!selectedDie) {
            return signalProcessor->show_error_message(
                "You have to select a die before using this tool card!");
        }

        // Has to create pointer because the PutDieWithoutNeighborTC::Move
        // deletes it in the destructor
        DieToFieldMove dtfm {game.get_curr_state()->player_on_move_id(),
                             selectedDie, boardInd,
                             DTFM_MODE_ACCEPT_UNREACHABLE};
        auto newMove = new PutDieWithoutNeighborTC::Move(
            game.get_curr_state()->player_on_move_id(), ind, dtfm);
        signalProcessor->send_move_request(newMove, moveResultHandlingInfo);
    }

    virtual void reset() override { selectedDie = nullptr; }

  private:
    PutDieWithoutNeighborTC& tc;
    Die* selectedDie;

    constexpr static const MoveResultHandlingPolicy moveResultHandlingInfo {
        .deselectOnFailure = true,
        .deselectOnSuccess = true};
};

#endif // PUTDIEWITHOUTNEIGHBORTCVIEW_HPP