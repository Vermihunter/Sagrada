#ifndef RULES_BASED_PLAYER_H
#define RULES_BASED_PLAYER_H

#include "AI_Player.h"
#include "HeuristicConstants.h"

using heuristic_value_t = int;
using move_eval_t       = std::pair<move_t, heuristic_value_t>;

/**
 * @brief An AI player based on the rules of the game
 *
 * It has a configurable mode:
 * - ALL_MOVES checks all the moves - stronger but slower
 * - ONLY_DTFM checks only DTFM moves (other are not even generated) - weaker
 * but faster
 */
class RulesBasedPlayer : public AI_Player {
  public:
	enum strategy_t { ONLY_DTFM, ALL_MOVES };
	static constexpr strategy_t DEFAULT_STRATEGY = ALL_MOVES;

	/** Default constructor */
	RulesBasedPlayer(const AIPlayerContext& _ctx, strategy_t strategy);
	virtual ~RulesBasedPlayer();

	/**
	 * @brief Chooses the Window pattern card with the most fields having the
	 * same color restriction as the color of the private objective card
	 */
	virtual ID_t choose_window_pattern_card(wpc_it_t iBegin, wpc_it_t iEnd) override;

	/**
	 * @brief Implements the algorithm based on the rules of the game
	 * Categorizes moves into groups and according to a priority configuration
	 * checks if the given groups contain a strong move and if so, sends it.
	 *
	 * @return move_t
	 */
	virtual move_t make_next_move() override;

	/**
	 * @brief Gets the possible moves according to the defined strategy
	 *
	 * @param g Game representing the state
	 * @return move_c The container of the possible moves
	 */
	virtual move_c get_possible_moves(Game& g) override;

  private:
	HeuristicConstants constants;
	strategy_t         strategy;

	/**
	 * @brief Heuristic evaluation of a state
	 *
	 * @param evalState State to evaluate
	 * @return heuristic_value_t Heuristic value of the state
	 */
	heuristic_value_t evaluate_puoc_state(const eval_state_t& evalState);

	/**
	 * @brief Get the best dtfm object
	 * Tries out the DTFM and chooses the one producing the best result.
	 *
	 * @param g Game representing the state
	 * @param dieToFieldMoves Container of DTFM moves
	 * @param evalStates Sent to gain the original state to compare with the new
	 * ones
	 * @return move_eval_t Pair of the best move and the corresponding heuristic
	 * value
	 */
	move_eval_t get_best_dtfm(Game& g, move_c& dieToFieldMoves, const eval_state_c& evalStates);

	/**
	 * @brief Checks whether a strong relocating move exists or not
	 *
	 * @param g Game representing the state
	 * @param moves Container of relocating moves
	 * @param evalStates Sent to gain the original state to compare with the new
	 * ones
	 * @return move_t The strongest relocating move or nullptr in case there is
	 * no strong relocating move
	 */
	move_t check_good_relocating_exists(Game& g, move_c& moves, const eval_state_c& evalStates);

	/**
	 * @brief Gets the best move when using the ONLY_DTFM strategy
	 *
	 * @return move_t
	 */
	move_t get_best_only_dtfm_move();
};

#endif // RULES_BASED_PLAYER_H