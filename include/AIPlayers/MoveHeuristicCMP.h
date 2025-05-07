#ifndef MOVE_HEURISTIC_CMP_H
#define MOVE_HEURISTIC_CMP_H

#include "Attributes.h"
#include "Board.h"
#include "DieToFieldMove.h"
#include "Typedefs.h"

#include <utility>

/**
 * @brief Heuristic sort comparator that is designed to be used with std::sort
 *
 * The following criteria are considered:
 * - If none of the moves is a DTFM -> the one with higher index is chosen
 * - If one of them is a DTFM, it is prioritized
 * - If both the moves are DTFM, the restrictions, the possible proc color and
 * the value of the dice are checked
 */
struct MoveHeuristicCMP {
	MoveHeuristicCMP(Board& _board, color_t _procColor) : board(_board), procColor(_procColor) {}

	bool operator()(const std::pair<move_t, size_t>& lhsMoveByInd,
	                const std::pair<move_t, size_t>& rhsMoveByInd)
	{
		auto& [lhsMove, lhsMoveInd] = lhsMoveByInd;
		auto& [rhsMove, rhsMoveInd] = rhsMoveByInd;

		if (!lhsMove) {
			return false;
		}
		if (!rhsMove) {
			return true;
		}

		if (!lhsMove || !rhsMove) {
			return false;
		}

		auto dtfm1 = lhsMove->get_placing_submove();
		auto dtfm2 = rhsMove->get_placing_submove();

		// None of them is dtfm move -> random but deterministic
		if (!dtfm1 && !dtfm2) {
			return lhsMoveInd < rhsMoveInd;
		}

		// lhsMove is dtfm, rhsMove is not -> prioritise lhsMove
		if (dtfm1 && !dtfm2) {
			return true;
		}
		// lhsMove is not dtfm, rhsMove is dtfm -> prioritise rhsMove
		if (dtfm2 && !dtfm1) {
			return false;
		}

		bool dtfm1ToRestrictionField =
		    board[dtfm1->boardIndices]->get_color_restriction().has_value() ||
		    board[dtfm1->boardIndices]->get_value_restriction().has_value();

		bool dtfm2ToRestrictionField =
		    board[dtfm2->boardIndices]->get_color_restriction().has_value() ||
		    board[dtfm2->boardIndices]->get_value_restriction().has_value();

		bool dtfm1ProcColored = (dtfm1->die->get_color() == procColor);
		bool dtfm2ProcColored = (dtfm2->die->get_color() == procColor);

		// lhsMove has die of same attribute color as the PRoC color, rhs not ->
		// prioritise lhsMove
		if (dtfm1ProcColored && !dtfm2ProcColored) {
			return true;
		}
		// lhsMove has not die of same attribute color as the PRoC color, rhs
		// has -> prioritise rhsMove
		if (!dtfm1ProcColored && dtfm2ProcColored) {
			return false;
		}

		// DTFM1 is placing a die to a field with restriction -> higher value
		// than placing a die to a field without restriction
		if (dtfm1ToRestrictionField && !dtfm2ToRestrictionField) {
			return true;
		}

		// DTFM2 is placing a die to a field with restriction -> higher value
		// than placing a die to a field without restriction
		if (dtfm2ToRestrictionField && !dtfm1ToRestrictionField) {
			return false;
		}

		return int(dtfm1->die->get_value()) > int(dtfm2->die->get_value());
	}

  private:
	Board&  board;
	color_t procColor;
};

#endif // MOVE_HEURISTIC_CMP_H