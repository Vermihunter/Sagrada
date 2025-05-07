#include "MinimaxPlayer.h"

#include "BoardFieldNeighborCache.h"
#include "Deallocator.h"
#include "Debugger.h"
#include "HeuristicFilter.h"
#include "RelocateDieMove.h"
#include "RelocateTwoDiceMove.h"

#include <sstream>

MinimaxPlayer::MinimaxPlayer(const AIPlayerContext& _ctx, size_t _depth,
                             const HeuristicConstants& _constants, int _worldCount)
    : AI_Player(_ctx), depth(_depth), constants(_constants), worldCount(_worldCount)
{
	minimizingPlayer.otherPlayer = &maximizingPlayer;
	maximizingPlayer.otherPlayer = &minimizingPlayer;
}

ID_t MinimaxPlayer::choose_window_pattern_card(wpc_it_t iBegin, wpc_it_t iEnd)
{
	return 0;
}

move_t MinimaxPlayer::make_next_move()
{
	if (ctx.gameDeterministic) {
		return minimax_algorithm(ctx.game, depth, INT_MIN, INT_MAX, maximizingPlayer, nullptr)
		    .first;
	}

	// Constructing the worlds with random future and random PrOC for other
	// players
	std::vector<std::pair<move_t, int>> overallResults;
	game_t                              firstSampleWorld;
	for (int i = 0; i < worldCount; ++i) {
		game_t sampleWorld = ctx.game.clone_with_pseudo_random_future(ctx.ID, i + 99);

		std::vector<std::pair<move_t, int>> currWorldResults;
		minimax_algorithm(*sampleWorld, depth, INT_MIN, INT_MAX, maximizingPlayer,
		                  &currWorldResults);

		if (overallResults.empty()) {
			overallResults   = currWorldResults;
			firstSampleWorld = std::move(sampleWorld);
			continue;
		}

		size_t moveInd = 0;
		for (auto&& [sampleWorldMove, sampleWorldMoveHeuristicValue] : currWorldResults) {
			overallResults[moveInd].second += sampleWorldMoveHeuristicValue;
			++moveInd;

			delete sampleWorldMove;
		}
	}

	move_t             bestMove          = nullptr;
	heuristic_result_t currBestHeuristic = HEURISTIC_MIN;
	for (auto&& [move, heuristicValue] : overallResults) {
		if (heuristicValue > currBestHeuristic) {
			bestMove          = move;
			currBestHeuristic = heuristicValue;
		}
	}

	move_t clonedMove =
	    bestMove->clone(*firstSampleWorld->get_ctx()->supply, *ctx.game.get_ctx()->supply);
	free_all_with_ind(overallResults.begin(), overallResults.end());
	return clonedMove;
}

heuristic_result_t MinimaxPlayer::get_heuristic_value_for_score(const eval_state_t& evalState)
{
	int puocUncompletablePoints = 0;
	int weightedPuocCompletable = 0;

	for (auto&& puocPattern : evalState->get_puoc_state()) {
		auto satisfactionValue = puocPattern.puoc.get_ctx()->get_satisfaction_value();
		for (auto&& subpattern : puocPattern.completableSubPatterns) {
			weightedPuocCompletable +=
			    (satisfactionValue *
			     std::pow((puocPattern.dicePerPattern - subpattern.diceToComplete) + 1,
			              constants.puocCompletablePower));
		}

		puocUncompletablePoints += (puocPattern.uncompletableSubPatternCount * satisfactionValue);
	}

	return (constants.completedPointsWeight * evalState->get_total_points()) +
	       (weightedPuocCompletable) -
	       (constants.minusPerUncompletablePuocPoints * puocUncompletablePoints) -
	       (evalState->get_overall_board_state().uncompletableFieldCount *
	        constants.minusPointsPerUncompletableField);
}

heuristic_result_t MinimaxPlayer::heuristic(Game& g)
{
	heuristic_result_t heuristicValue = 0;
	eval_state_c       scores         = g.evaluate();

	if (g.get_curr_state()->hasEnded) {
		// The minimax player has the highest points in the game
		if (scores[0]->get_player()->ID == ctx.ID) {
			heuristicValue = HEURISTIC_MAX / 256;
		}
		else {
			heuristicValue = HEURISTIC_MIN / 256;
		}

		return heuristicValue;
	}

	for (auto&& score : scores) {
		heuristic_result_t playerScoreHeuristic = get_heuristic_value_for_score(score);
		if (score->get_player()->ID == ctx.ID) {
			heuristicValue += playerScoreHeuristic;
		}
		else {
			heuristicValue -= (playerScoreHeuristic / constants.opponentHeuristicInfluencingFactor);
		}
	}

	return heuristicValue;
}

move_by_index_c& MinimaxPlayer::get_moves(Game& g, move_c& allMoves, move_by_index_c& bestMoves,
                                          move_by_index_c& backupMoves)
{
	filter_moves(g, allMoves, bestMoves, backupMoves);

	/** Heuristic sort */
	move_by_index_c& movesToDiscover = (bestMoves.empty() ? backupMoves : bestMoves);
	auto&            currPlayer = *g.get_ctx()->players[g.get_curr_state()->player_on_move_id()];
	auto&            board      = *currPlayer.board;
	color_t          currPlayerProcColor = currPlayer.PrOC->get_color();
	std::sort(movesToDiscover.begin(), movesToDiscover.end(),
	          MoveHeuristicCMP{board, currPlayerProcColor});

	return movesToDiscover;
}

std::pair<move_t, heuristic_result_t> MinimaxPlayer::minimax_algorithm(
    Game& g, size_t _depth, int alpha, int beta, MinimaxPlayerType<heuristic_result_t>& player,
    std::vector<std::pair<move_t, heuristic_result_t>>* movesWithHeuristicValue)
{
	if (_depth == 0 || g.get_curr_state()->hasEnded) return {nullptr, heuristic(g)};

	move_t             bestMove          = nullptr;
	heuristic_result_t currBestPlayerVal = player.get_initial_value();

	move_c possibleMoves = g.possible_moves();

	move_by_index_c backupMovesByIndices;
	move_by_index_c heuristicFilteredMovesByIndices;

	/** Heuristic filter & sort */
	move_by_index_c& movesToDiscover =
	    get_moves(g, possibleMoves, heuristicFilteredMovesByIndices, backupMovesByIndices);
	if (movesWithHeuristicValue) {
		movesWithHeuristicValue->reserve(movesToDiscover.size());
	}

	for (auto&& [move, moveInd] : movesToDiscover) {
		if (_depth == 1 && move->t == Move::type::TOOL_CARD &&
		    !((tool_card_move_t)(move))->consists_of_multiple_submoves().isSingleMoveToolCard) {
			continue;
		}

		g.move_request(move);

		// Some moves consists of more than one submove and between the submoves
		// the other player cannot react to the first submove so changing the
		// player type only when the sent move has shifted the turn. Also it
		// happens that the player moves twice in a row
		bool maximizingPlayerNextMove = (ctx.ID == g.get_curr_state()->player_on_move_id());
		MinimaxPlayerType<heuristic_result_t>* nextLayerPlayer =
		    (maximizingPlayerNextMove
		         ? (MinimaxPlayerType<heuristic_result_t>*)(&maximizingPlayer)
		         : (MinimaxPlayerType<heuristic_result_t>*)(&minimizingPlayer));

		auto [returnedMove, heuristicValue] =
		    minimax_algorithm(g, _depth - 1, alpha, beta, *nextLayerPlayer, nullptr);

		if (movesWithHeuristicValue) {
			movesWithHeuristicValue->push_back({move, heuristicValue});
		}

		g.undo_last_move();

		if (player.is_better_value(heuristicValue, currBestPlayerVal)) {
			currBestPlayerVal = heuristicValue;
			bestMove          = move;
		}

		player.update_alpha_beta(alpha, beta, heuristicValue);

		// Alpha-beta cutoff
		if (beta <= alpha) break;
	}

	// Some constants may result in not choosing any move
	if (depth == _depth && !bestMove) {
		bestMove          = possibleMoves[0];
		currBestPlayerVal = 0;
	}

	if (depth != _depth) {
		free_all(possibleMoves.begin(), possibleMoves.end());
	}
	else if (!movesWithHeuristicValue) {
		free_all_but(possibleMoves.begin(), possibleMoves.end(), bestMove);
	}
	else {
		move_by_index_c& unusedMoves =
		    (&movesToDiscover == &backupMovesByIndices ? heuristicFilteredMovesByIndices
		                                               : backupMovesByIndices);
		free_all_with_ind(unusedMoves.begin(), unusedMoves.end());
	}

	return {bestMove, currBestPlayerVal};
}
