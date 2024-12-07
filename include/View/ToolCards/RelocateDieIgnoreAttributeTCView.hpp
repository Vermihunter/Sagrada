#ifndef RELOCATEDIEIGNOREATTRIBUTETCVIEW_HPP
#define RELOCATEDIEIGNOREATTRIBUTETCVIEW_HPP

#include <memory>

#include "Move.h"
#include "RectangularBoardView.h"
#include "RelocateDieIgnoreAttributeTC.h"
#include "RelocateDieMove.h"
#include "RelocatingToolCardView.hpp"
#include "ToolCardView.h"

template <dtfm_mode_t Attribute>
class RelocateDieIgnoreAttributeTCView : public RelocatingToolCardView
{
    constexpr static dtfm_mode_mask_t mask_attr = dtfm_mode_mask_t(Attribute);

  public:
    RelocateDieIgnoreAttributeTCView(ToolCardViewContainer& _parentContainer,
                                     Game& _game, ToolCard& _tc,
                                     const SizeInPixels& size, size_t ind)
        : RelocatingToolCardView(_parentContainer, _game, _tc, size, ind)
        , tc((RelocateDieIgnoreAttributeTC<Attribute>&)(_tc))
    {
    }

    virtual void receive_board_field_click_signal(
        RectangularBoardView* boardView, const BoardIndex& boardInd) override
    {
        if (!fromField) {
            return process_from_field(boardView, boardInd);
        }

        ID_t playerOnMoveID = game.get_curr_state()->player_on_move_id();
        auto move = new RelocateDieMove(playerOnMoveID, ind, *fromField,
                                        boardInd, mask_attr);
        signalProcessor->send_move_request(move, moveResultHandlingInfo);
    }

    virtual void reset() override { fromField = nullptr; }

  private:
    RelocateDieIgnoreAttributeTC<Attribute>& tc;

    constexpr static const MoveResultHandlingPolicy moveResultHandlingInfo {
        .deselectOnFailure = true,
        .deselectOnSuccess = true};

    void process_from_field(RectangularBoardView* boardView,
                            const BoardIndex& boardInd)
    {
        move_c usableMoves =
            try_make_ind_from_field(tc, boardView, boardInd, nullptr);
        if (usableMoves.empty()) {
            reset();
            return signalProcessor->show_error_message(
                "There is no field where you could move that die!");
        }

        fromField = std::make_unique<BoardIndex>(boardInd);
        board_ind_c placeableIndices;
        for (auto&& move : usableMoves) {
            relocate_move_t relocMove = (RelocateDieMove*)(move);
            if (relocMove->from == *fromField) {
                placeableIndices.push_back(relocMove->to);
            }
        }

        signalProcessor->highlight_fields(
            placeableIndices, game.get_curr_state()->player_on_move_id());
        free_all(usableMoves.begin(), usableMoves.end());
    }
};

#endif // RELOCATEDIEIGNOREATTRIBUTETCVIEW_HPP