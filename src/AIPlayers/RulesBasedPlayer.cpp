#include "RulesBasedPlayer.h"

#include "Deallocator.h"
#include "PassMove.h"

#include <algorithm>

RulesBasedPlayer::RulesBasedPlayer(const AIPlayerContext& _ctx, strategy_t _strategy)
    : AI_Player(_ctx), constants(defaultConstants), strategy(_strategy)
{
	// ctor
}

ID_t RulesBasedPlayer::choose_window_pattern_card(wpc_it_t iBegin, wpc_it_t iEnd)
{
	auto fieldRestrictionMatchesProcColor =
	    [&](const auto& field) { // std::pair<BoardIndex, board_field_t>
		    return field->get_color_restriction().has_value() &&
		           field->get_color_restriction().value() == ctx.privateObjectiveCard->get_color();
	    };

	auto getRestrictionFieldCount = [&](auto& board) {
		size_t count = 0;
		for (auto&& row : board->data) {
			for (auto&& field : row) {
				if (fieldRestrictionMatchesProcColor(field)) {
					++count;
				}
			}
		}

		return count;
	};

	size_t wpcInd                   = 0;
	size_t maxProcRestrictionFields = 0;
	size_t ind                      = 0;
	for (auto wpcIt = iBegin; wpcIt != iEnd; ++wpcIt) {
		size_t procRestrictionFieldCount = getRestrictionFieldCount(*wpcIt);
		if (procRestrictionFieldCount > maxProcRestrictionFields) {
			maxProcRestrictionFields = procRestrictionFieldCount;
			wpcInd                   = ind;
		}
		++ind;
	}

	return wpcInd;
}

move_c RulesBasedPlayer::get_possible_moves(Game& g)
{
	if (strategy == strategy_t::ALL_MOVES || ctx.game.get_curr_state()->previousToolCardMove) {
		return g.possible_moves();
	}

	// At this point we are sure that the strategy is ONLY_DTFM and it is not a
	// submove of a previously used tool card (MCTS playout)
	move_c allDtfmMoves;
	ctx.game.get_ctx()->basicMoveHandler->append_basic_moves(allDtfmMoves, ctx.game, ctx.ID,
	                                                         dtfm_mode_t::DTFM_MODE_DEFAULT);
	allDtfmMoves.push_back(PassMove::create(ctx.ID));
	return allDtfmMoves;
}

move_t RulesBasedPlayer::get_best_only_dtfm_move()
{
	// The previous submove was a tool card that consists of multiple sub-moves.
	// In this case, you cannot use a dtfm, but only a next sub-move of the tool
	// card. This may happen only if the player's strategy is used indirectly
	// e.g. as a playout strategy for the MCTS player.
	if (ctx.game.get_curr_state()->previousToolCardMove) {
		strategy_t origStrategy = strategy;
		strategy                = RulesBasedPlayer::ALL_MOVES;
		move_t tcMove           = make_next_move();
		strategy                = origStrategy;
		return tcMove;
	}

	move_c allDtfmMoves;
	ctx.game.get_ctx()->basicMoveHandler->append_basic_moves(allDtfmMoves, ctx.game, ctx.ID,
	                                                         dtfm_mode_t::DTFM_MODE_DEFAULT);
	if (allDtfmMoves.empty()) {
		return PassMove::create(ctx.ID);
	}

	eval_state_c scores = ctx.game.evaluate();

	move_eval_t bestDtfm = get_best_dtfm(ctx.game, allDtfmMoves, scores);
	free_all_but(allDtfmMoves.begin(), allDtfmMoves.end(), bestDtfm.first);

	return bestDtfm.first;
}

move_t RulesBasedPlayer::make_next_move()
{
	if (strategy == ONLY_DTFM) {
		return get_best_only_dtfm_move();
	}

	auto         all_moves = ctx.game.possible_moves();
	eval_state_c scores    = ctx.game.evaluate();

	move_c positionBetteringToolCardMoves;
	move_c randomContainingToolCardMoves;

	move_c dieToFieldMoves;

	for (auto&& move : all_moves) {
		if (move->is_relocating()) {
			positionBetteringToolCardMoves.push_back(move);
		}
		else if (move->contains_randomness()) {
			randomContainingToolCardMoves.push_back(move);
		}
		else {
			dieToFieldMoves.push_back(move);
		}
	}

	auto positionBetteringRelocatingMove =
	    check_good_relocating_exists(ctx.game, positionBetteringToolCardMoves, scores);
	if (positionBetteringRelocatingMove) {
		free_all_but(all_moves.begin(), all_moves.end(), positionBetteringRelocatingMove);
		return positionBetteringRelocatingMove;
	}

	auto [bestDtfm, dtfmHeuristic] = get_best_dtfm(ctx.game, dieToFieldMoves, scores);

	move_t bestMove = bestDtfm;
	if (!bestMove || dtfmHeuristic < 0) {
		if (!randomContainingToolCardMoves.empty()) {
			bestMove = randomContainingToolCardMoves[ctx.game.get_ctx()->rnd->random_in_interval(
			    0, randomContainingToolCardMoves.size() - 1)];
		}
		else {
			bestMove = all_moves[0];
		}
	}

	free_all_but(all_moves.begin(), all_moves.end(), bestMove);

	return bestMove;
}

move_t RulesBasedPlayer::check_good_relocating_exists(Game& g, move_c& moves,
                                                      const eval_state_c& scores)
{
	move_t              bestMove                = nullptr;
	int                 currMostFreedFieldCount = 0;
	const eval_state_t& myScore =
	    *std::find_if(scores.begin(), scores.end(), ScoreByPlayerId{ctx.ID});

	for (auto&& move : moves) {
		if (move->t == Move::type::TOOL_CARD &&
		    !((tool_card_move_t)(move))->consists_of_multiple_submoves().isSingleMoveToolCard) {
			continue;
		}

		g.move_request(move);

		eval_state_c clonedScores = g.evaluate();
		auto&        newMyScore =
		    *std::find_if(clonedScores.begin(), clonedScores.end(), ScoreByPlayerId{ctx.ID});

		int uncompletableFieldCountDifference =
		    (int)(myScore->get_overall_board_state().uncompletableFieldCount) - // Original
		    (int)(newMyScore->get_overall_board_state()
		              .uncompletableFieldCount); // After the relocating move

		if (uncompletableFieldCountDifference > currMostFreedFieldCount) {
			currMostFreedFieldCount = uncompletableFieldCountDifference;
			bestMove                = move;
		}

		g.undo_last_move();
	}

	return bestMove;
}

move_eval_t RulesBasedPlayer::get_best_dtfm(Game& g, move_c& dieToFieldMoves,
                                            const eval_state_c& evalStates)
{
	if (dieToFieldMoves.empty()) {
		return {nullptr, -1};
	}

	const eval_state_t& myScore =
	    *std::find_if(evalStates.begin(), evalStates.end(), ScoreByPlayerId{ctx.ID});

	// the last two parts should be multiplied with the corresponding evaluation
	// constants from the score context object
	int totalPoints = myScore->get_total_points() - myScore->get_empty_fields_points() -
	                  myScore->get_overall_board_state().uncompletableFieldCount;
	int               currMinNewUnfillableFieldCount = INT_MIN;
	heuristic_value_t bestStateValue                 = 0;
	move_eval_t       chosenMove{dieToFieldMoves[0], bestStateValue};
	for (auto&& move : dieToFieldMoves) {
		auto dtfm = move->get_placing_submove();
		// Pass move
		if (!dtfm) {
			continue;
		}

		g.move_request(move);

		eval_state_c clonedScores = g.evaluate();
		g.undo_last_move();
		auto& newMyScore =
		    *std::find_if(clonedScores.begin(), clonedScores.end(), ScoreByPlayerId{ctx.ID});

		int uncompletableFieldCountDifference =
		    (int)(myScore->get_overall_board_state().uncompletableFieldCount) -   // Original
		    (int)(newMyScore->get_overall_board_state().uncompletableFieldCount); // After move

		// There was a move before that produced less new uncompletable fields
		if (uncompletableFieldCountDifference < currMinNewUnfillableFieldCount) {
			continue;
		}

		currMinNewUnfillableFieldCount =
		    std::max(currMinNewUnfillableFieldCount, uncompletableFieldCountDifference);
		heuristic_value_t newStateValue = evaluate_puoc_state(newMyScore);
		// Prioritising moves that would place a die with the privat objective
		// card color
		if (dtfm->die->get_color() == ctx.privateObjectiveCard->get_color()) {
			newStateValue += (((heuristic_value_t)(dtfm->die->get_value()) + 1) * 5);
		}

		int newTotalPoints = newMyScore->get_total_points() -
		                     newMyScore->get_empty_fields_points() -
		                     newMyScore->get_overall_board_state().uncompletableFieldCount;

		// Either the new puoc state is directly better than the old one
		// or it is almost as good but it brings more already achieved points to
		// the table
		if (newStateValue > bestStateValue ||
		    ((newStateValue + 10) > bestStateValue && (newTotalPoints > (totalPoints + 3)))) {
			bestStateValue = newStateValue;
			chosenMove     = move_eval_t{move, newStateValue};
		}
	}

	return chosenMove;
}

heuristic_value_t RulesBasedPlayer::evaluate_puoc_state(const eval_state_t& evalState)
{
	int puocUncompletablePoints = 0;
	int weightedPuocCompletable = 0;
	int puocPointsCompleted     = 0;

	for (auto&& puocPattern : evalState->get_puoc_state()) {
		auto satisfactionValue = puocPattern.puoc.get_ctx()->get_satisfaction_value();
		for (auto&& subpattern : puocPattern.completableSubPatterns) {
			weightedPuocCompletable +=
			    (satisfactionValue *
			     std::pow((puocPattern.dicePerPattern - subpattern.diceToComplete) + 1,
			              constants.puocCompletablePower));
		}

		puocUncompletablePoints += (puocPattern.uncompletableSubPatternCount * satisfactionValue);
		puocPointsCompleted += puocPattern.totalEarnedPoints;
	}

	return (constants.completedPointsWeight * puocPointsCompleted) + (weightedPuocCompletable) -
	       (constants.minusPerUncompletablePuocPoints * puocUncompletablePoints);
}

RulesBasedPlayer::~RulesBasedPlayer()
{
	// dtor
}
