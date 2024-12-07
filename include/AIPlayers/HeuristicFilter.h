#ifndef HEURISTIC_FILTER_H
#define HEURISTIC_FILTER_H

#include "BoardFieldNeighborCache.h"
#include "Game.h"
#include "RelocateDieMove.h"
#include "RelocateTwoDiceMove.h"
#include "Typedefs.h"

/**
 * @addtogroup HeuristicFilters
 * These functions are used for filtering out the weak moves according to a
 * domain specific approach.
 * @{
 * Defines the logic for filtering a DieToFieldMove.
 *
 * Checks if any of the direct neighbors has the same attribute restriction as
 * the placeable dice's attribute.
 *
 * @param board The board that represents the current state
 * @param move Move that is checkced for filtering
 * @return true If the move should be filtered i.e. considered as a weak move
 * @return false If the move should not be filtered i.e. not considered as a
 * weak move
 */
inline bool filter_die_to_field_move(Board& board, DieToFieldMove& move)
{
    for (auto&& neighborInd :
         BoardFieldNeighborCache::GetDirectNeighborIndicesFor(
             move.boardIndices)) {
        auto& neighborField = board[neighborInd];
        if (!neighborField->get_die() &&
            ((neighborField->get_color_restriction().has_value() &&
              neighborField->get_color_restriction().value() ==
                  move.die->get_color()) ||
             (neighborField->get_value_restriction().has_value() &&
              neighborField->get_value_restriction().value() ==
                  move.die->get_value()))) {
            return true;
        }
    }

    return false;
}

/**
 * @brief Wrapper for adding the move to the correct container according to the
 * fact whether the move will be filtered or not
 *
 * @param goodMoves The container where the non-filtered moves are appended
 * @param backupMoves The container where the filtered moves are appended
 * @param board The board that represents the current state
 * @param dtfm The move to check for filtering (may be temporary that is why it
 * is separated from the next argument)
 * @param move The move to be appended to the correct container
 */
inline void insert_dtfm(move_by_index_c& goodMoves,
                        move_by_index_c& backupMoves, Board& board,
                        DieToFieldMove& dtfm, move_t move)
{
    if (filter_die_to_field_move(board, dtfm)) {
        backupMoves.push_back({move, backupMoves.size()});
    }
    else {
        goodMoves.push_back({move, goodMoves.size()});
    }
}

/**
 * @brief The main function to call to separate the filtered moves
 *
 * @param g The game representing the context
 * @param allMoves Vector of all the possible moves to separate
 * @param bestMoves The moves considered to be the stronger ones
 * @param backupMoves The moves considered to be the weaker ones
 */
inline void filter_moves(Game& g, const move_c& allMoves,
                         move_by_index_c& bestMoves,
                         move_by_index_c& backupMoves)
{
    bestMoves.reserve(allMoves.size());
    backupMoves.reserve(allMoves.size());

    auto& currPlayer =
        *g.get_ctx()->players[g.get_curr_state()->player_on_move_id()];
    auto& board = *currPlayer.board;
    for (auto&& move : allMoves) {
        if (auto dtfm = move->get_placing_submove()) {
            insert_dtfm(bestMoves, backupMoves, board, *dtfm, move);
        }
        else if (auto relocMove = dynamic_cast<RelocateDieMove*>(move)) {
            DieToFieldMove tmpDtfm {0, board[relocMove->from]->get_die(),
                                    relocMove->to, DTFM_MODE_DEFAULT};
            insert_dtfm(bestMoves, backupMoves, board, tmpDtfm, move);
        }
        else if (auto twoRelocMove = dynamic_cast<RelocateTwoDiceMove*>(move)) {
            DieToFieldMove tmpDtfm1 {0, board[twoRelocMove->m1.from]->get_die(),
                                     twoRelocMove->m1.to, DTFM_MODE_DEFAULT};
            DieToFieldMove tmpDtfm2 {0, board[twoRelocMove->m2.from]->get_die(),
                                     twoRelocMove->m2.to, DTFM_MODE_DEFAULT};
            if (!filter_die_to_field_move(board, tmpDtfm1) &&
                (!twoRelocMove->hasSecondRelocateMove ||
                 !filter_die_to_field_move(board, tmpDtfm2))) {
                bestMoves.push_back({move, bestMoves.size()});
            }
            else {
                backupMoves.push_back({move, backupMoves.size()});
            }
        }
        else if (move->t == Move::type::TOOL_CARD) {
            bestMoves.push_back({move, backupMoves.size()});
        }
        else {
            // Pass move
            bestMoves.push_back({move, backupMoves.size()});
        }
    }
}
/**
 * @}
 */

#endif // HEURISTIC_FILTER_H