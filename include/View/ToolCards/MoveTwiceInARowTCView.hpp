#ifndef MOVETWICEINAROWTCVIEW_HPP
#define MOVETWICEINAROWTCVIEW_HPP

#include "MoveTwiceInARowTC.h"
#include "ToolCardView.h"

class MoveTwiceInARowTCView : public ToolCardView
{
  public:
    MoveTwiceInARowTCView(ToolCardViewContainer& _parentContainer, Game& game,
                          ToolCard& _tc, const SizeInPixels& size, size_t ind)
        : ToolCardView(_parentContainer, game, _tc, size, ind)
        , tc((MoveTwiceInARowTC&)(_tc))
        , moveInProgress(false)
    {
    }

    virtual bool set_selected(bool _isSelected) override
    {
        if (moveInProgress) {
            return !_isSelected;
        }

        moveInProgress = true;
        auto move = new MoveTwiceInARowTC::Move(
            game.get_curr_state()->player_on_move_id(), ind);
        signalProcessor->send_move_request(move, moveResultHandlingInfo);

        da_CostVisualizer->queue_draw();
        moveInProgress = false;

        return false;
    }

  private:
    MoveTwiceInARowTC& tc;
    bool moveInProgress;

    constexpr static const MoveResultHandlingPolicy moveResultHandlingInfo {
        .deselectOnFailure = true,
        .deselectOnSuccess = true};
};

#endif // MOVETWICEINAROWTCVIEW_HPP