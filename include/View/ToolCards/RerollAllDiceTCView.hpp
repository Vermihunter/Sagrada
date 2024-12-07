#ifndef REROLLALLDICETCVIEW_HPP
#define REROLLALLDICETCVIEW_HPP

#include "RerollAllDiceTC.h"
#include "ToolCardView.h"

class RerollAllDiceTCView : public ToolCardView
{
  public:
    RerollAllDiceTCView(ToolCardViewContainer& _parentContainer, Game& game,
                        ToolCard& tc, const SizeInPixels& size, size_t ind)
        : ToolCardView(_parentContainer, game, tc, size, ind)
        , moveInProgress(false)
    {
    }

    virtual bool set_selected(bool _isSelected) override
    {
        // Prevents recursive calls at deselecting
        if (moveInProgress) {
            return false;
        }

        moveInProgress = true;
        auto move = new RerollAllDiceTC::Move(
            game.get_curr_state()->player_on_move_id(), ind);
        signalProcessor->send_move_request(move, moveResultHandlingInfo);

        da_CostVisualizer->queue_draw();
        moveInProgress = false;

        return false;
    }

  private:
    bool moveInProgress;
    constexpr static const MoveResultHandlingPolicy moveResultHandlingInfo {
        .deselectOnFailure = true,
        .deselectOnSuccess = true};
};

#endif // REROLLALLDICETCVIEW_HPP