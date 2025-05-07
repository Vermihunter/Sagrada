#include "OverallBoardState.h"

#include "Game.h"

OverallBoardState::OverallBoardState(const Game& g, const ID_t playerID)
{
	add_unfillable_field_indices(g, playerID);
}

void OverallBoardState::add_unfillable_field_indices(const Game& g, const ID_t playerID)
{
	auto&      bmHandler             = g.get_ctx()->basicMoveHandler;
	auto&      board                 = *GameInfo::get_player_with_id(g, playerID)->board;
	const auto placeabilityProcessor = board.get_ctx()->config->type->get_placeability_processor();

	BoardIndex ind{0, 0};
	for (auto&& row : board.get_ctx()->data) {
		for (auto&& field : row) {
			// We want to process only empty fields that are reachable
			if (field->get_die() || !bmHandler->is_reachable(board, ind)) {
				++ind.col;
				continue;
			}

			if (!placeabilityProcessor->any_placeable_die(board, ind)) {
				unfillableFieldIndices.push_back(ind);
			}

			++ind.col;
		}
		ind.col = 0;
		++ind.row;
	}

	size_t emptyFieldCount = board.get_number_of_empty_fields();
	size_t movesLeft       = (g.get_curr_state()->hasEnded)
	                             ? 0
	                             : (g.get_ctx()->numberOfRounds * 2 - // All moves in a game
                              (g.get_curr_state()->roundNumber *
                               2) + // Minus the two moves made in all previous rounds
                              g.get_curr_state()->moves_remaining_for_player_this_round(
	                                    playerID)); // Plus one if the player still has a move this
	                                                // round

	// A player can never place two dice in a single turn
	if (emptyFieldCount < movesLeft) {
		throw std::invalid_argument{"This should never happen"};
	}

	size_t passedMoves      = emptyFieldCount - movesLeft;
	uncompletableFieldCount = std::max(passedMoves, unfillableFieldIndices.size());
}