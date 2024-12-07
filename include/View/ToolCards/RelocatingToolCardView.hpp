#ifndef RELOCATING_TOOL_CARD_VIEW_HPP
#define RELOCATING_TOOL_CARD_VIEW_HPP

#include "RectangularBoardView.h"
#include "RelocateDieMove.h"
#include "RelocateTwoDiceMove.h"
#include "ToolCardView.h"
#include "Typedefs.h"

class RelocatingToolCardView : public ToolCardView
{
  public:
    virtual void receive_board_field_click_signal(
        RectangularBoardView* boardView, const BoardIndex& boardInd) override
    {
        if (!firstRelocateMove) {
            return handle_first_relocate_move_click(boardView, boardInd);
        }

        if (!fromField) {
            return process_from_field(boardView, boardInd);
        }

        ID_t playerOnMoveID = game.get_curr_state()->player_on_move_id();
        auto secondRelocateMove = std::make_shared<RelocateDieMove>(
            playerOnMoveID, ind, *fromField, boardInd);
        auto twoRelocateMoves =
            new RelocateTwoDiceMove(*firstRelocateMove, *secondRelocateMove);
        swap_at_ind(firstRelocateMove->from, firstRelocateMove->to);
        firstRelocateMove = nullptr;

        signalProcessor->send_move_request(twoRelocateMoves,
                                           moveResultHandlingInfo);
    }

    virtual void reset() override
    {
        if (firstRelocateMove) {
            swap_at_ind(firstRelocateMove->from, firstRelocateMove->to);
        }

        fromField = nullptr;
        firstRelocateMove = nullptr;
        usedBoardView = nullptr;
        free_all(cachedMoves.begin(), cachedMoves.end());
        cachedMoves.clear();
    }

  protected:
    using board_ind_ptr_t = std::unique_ptr<BoardIndex>;
    inline static Color cyanSelectColor {.r = 0, .g = 1.0, .b = 1.0, .a = 1.0};

    constexpr static const MoveResultHandlingPolicy moveResultHandlingInfo {
        .deselectOnFailure = true,
        .deselectOnSuccess = true};

    RelocatingToolCardView(ToolCardViewContainer& parentContainer, Game& _game,
                           ToolCard& _tc, const SizeInPixels& size, size_t _ind)
        : ToolCardView(parentContainer, _game, _tc, size, _ind)
        , tc(_tc)
        , firstRelocateMove(nullptr)
        , usedBoardView(nullptr)
    {
    }

    ToolCard& tc;
    std::unique_ptr<RelocateDieMove> firstRelocateMove;
    board_ind_ptr_t fromField;
    RectangularBoardView* usedBoardView;
    move_c cachedMoves;

    move_c try_make_ind_from_field(ToolCard& concreteToolCard,
                                   RectangularBoardView* boardView,
                                   const BoardIndex& boardInd,
                                   tool_card_move_t prevMove)
    {
        Die* fromFieldDie =
            boardView->get_ctx().data[boardInd.row][boardInd.col]->get_die();

        if (!fromFieldDie) {
            return {};
        }

        return collect_possible_moves(concreteToolCard, prevMove);
    }

    void swap_at_ind(const BoardIndex& ind1, const BoardIndex& ind2)
    {
        auto& field1 = (*usedBoardView)[ind1]->get_field();
        auto& field2 = (*usedBoardView)[ind2]->get_field();

        Die* die1 = field1.get_die();
        Die* die2 = field2.get_die();

        field2.put_die(die1);
        field1.put_die(die2);
        signalProcessor->redraw_curr_rounds_dice();
    }

    void handle_first_relocate_move_click(RectangularBoardView* boardView,
                                          const BoardIndex& boardInd)
    {
        if (!fromField) {
            usedBoardView = boardView;
            return process_from_field(boardView, boardInd);
        }

        auto searchRelatedRelocMoveFields = [&](move_t& cachedMove) {
            auto& relocMove = ((RelocateTwoDiceMove*)(cachedMove))->m1;
            return relocMove.from == *fromField && relocMove.to == boardInd;
        };

        auto usableFirstRelocateMoveIt =
            std::find_if(cachedMoves.begin(), cachedMoves.end(),
                         searchRelatedRelocMoveFields);
        if (usableFirstRelocateMoveIt == cachedMoves.end()) {
            return signalProcessor->show_error_message(
                "You cannot move that die there!");
        }

        firstRelocateMove = std::make_unique<RelocateDieMove>(
            game.get_curr_state()->player_on_move_id(), ind, *fromField,
            boardInd);
        swap_at_ind(*fromField, boardInd);
        boardView->unselect_all_fields();
        parentContainer.queue_draw_common_area();

        fromField = nullptr;
    }

    void process_from_field(RectangularBoardView* boardView,
                            const BoardIndex& boardInd)
    {
        if (!firstRelocateMove) {
            cachedMoves.clear();
            cachedMoves =
                try_make_ind_from_field(tc, boardView, boardInd, nullptr);
        }

        if (cachedMoves.empty()) {
            reset();
            return signalProcessor->show_error_message(
                "There is no field where you could move that die!");
        }

        board_ind_c placeableIndices;
        for (auto&& move : cachedMoves) {
            auto twoRelocMove = dynamic_cast<RelocateTwoDiceMove*>(move);
            auto& relocMove =
                (firstRelocateMove) ? twoRelocMove->m2 : twoRelocMove->m1;

            // The RelocateMaxTwoSameColorDiceTC uses this implementation as
            // well and there are moves that only relocate one die, therefore
            // the second relocation is not used
            if (!twoRelocMove->hasSecondRelocateMove) {
                continue;
            }

            // If its the second relocating move, we highlight only the ones
            // that correspond to the first relocating move and have the same
            // from value as the current click
            if (firstRelocateMove &&
                (twoRelocMove->m1.from != firstRelocateMove->from ||
                 twoRelocMove->m1.to != firstRelocateMove->to)) {
                continue;
            }

            if (relocMove.from == boardInd) {
                placeableIndices.push_back(relocMove.to);
            }
        }

        if (placeableIndices.empty()) {
            return signalProcessor->show_error_message(
                "There is no field where you could move that die!");
        }

        fromField = std::make_unique<BoardIndex>(boardInd);
        signalProcessor->highlight_fields(
            placeableIndices, game.get_curr_state()->player_on_move_id());
        (*boardView)[boardInd]->set_selected(&cyanSelectColor);
    }
};

#endif // RELOCATING_TOOL_CARD_VIEW_HPP