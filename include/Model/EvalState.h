#ifndef SCORE_H
#define SCORE_H

#include "Board.h"
#include "PrivateObjectiveCard.h"
#include "PublicObjectiveCard.h"
#include "PublicObjectiveCardPatternState.h"
#include "ScoreContext.h"

#include <memory>
#include <vector>

class Game;

/**
 * @brief Represents the score of a player after the game has ended
 *
 */
class EvalState {
	const player_t& player;

  public:
	EvalState(const player_t& player, Game& g);

	/**
	 * @brief Factory method that constructs score for a player
	 *
	 * @param player player that the score is constructed about
	 * @param g game object to ask for information about the chosen
	 * configuration
	 * @return std::unique_ptr<EvalState> unique pointer to the newly
	 * constructed EvalState object
	 */
	static eval_state_t for_player(const player_t& player, Game& g);
	static eval_state_c for_players(Game& g);

	/** Getters for different types of points */
	auto        get_total_points() const { return totalPoints; }
	auto        get_proc_points() const { return privateObjectiveCardPoints; }
	int         get_empty_fields_points() const { return (-1) * emptyFieldMinusPoints; }
	auto        get_unused_favor_token_points() const { return unusedFavorTokenPoints; }
	const auto& get_puoc_state() const { return puocPatterns; }
	const auto& get_player() const { return player; }
	const auto& get_overall_board_state() const { return boardState; }
	auto&       get_puoc() const { return puoc; }

  private:
	score_ctx_t& ctx;
	puoc_c&      puoc;
	/** @addtogroup EvaluationHelpers
	 * @brief Helper functions to evaluate different parts
	 *
	 * @{
	 */
	void evaluate_favor_token_points();
	void evaluate_empty_fields();
	void evaluate_private_objective_card_points();
	void evaluate_public_objective_card_points(puoc_c& puocs);
	/** @} */

	/** @addtogroup ScoreComponents
	 * @brief Components that make up the score
	 *
	 * @{
	 */
	int                           emptyFieldMinusPoints;
	size_t                        unusedFavorTokenPoints;
	std::vector<PuocPatternState> puocPatterns;
	size_t                        privateObjectiveCardPoints;
	int                           totalPoints;
	OverallBoardState             boardState;
	/** @} */
};

struct ScoreCmp {
	ScoreCmp() {}

	inline bool operator()(const std::pair<ID_t, eval_state_t>& lhs,
	                       const std::pair<ID_t, eval_state_t>& rhs) const
	{
		return lhs.second->get_total_points() >= rhs.second->get_total_points();
	}
};

struct ScoreByPlayerId {
	ScoreByPlayerId(ID_t _playerID);

	bool operator()(const eval_state_t& score);

  private:
	ID_t playerID;
};

#endif
