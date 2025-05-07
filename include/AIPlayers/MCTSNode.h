#ifndef MCTS_NODE_H
#define MCTS_NODE_H

#include "Deallocator.h"
#include "Game.h"
#include "HeuristicFilter.h"
#include "MCTSPlayerContext.h"
#include "MoveHeuristicCMP.h"

#include <memory>

using uct_t = double;

class MCTS_Node;
using mcts_node_t = std::unique_ptr<MCTS_Node>;

/**
 * @brief Represents a node in the MCTS algorithm
 *
 */
class MCTS_Node {
  public:
	MCTS_Node(game_t _game, MCTS_Node* _parent, const MCTSPlayerContext& _playerCtx);
	MCTS_Node(MCTS_Node&&) = default;

	~MCTS_Node();

	auto&            get_children() { return children; }
	move_by_index_c& get_moves() { return *possibleMoves; }
	game_t&          get_game() { return game; }
	MCTS_Node*       get_parent() { return parent; }

	/**
	 * @addtogroup SelectionStateMethods
	 * These functions are called during the selection process to query node
	 * states.
	 * @{
	 */
	bool is_fully_expanded() { return children.size() == possibleMoves->size(); }
	bool is_leaf() { return children.empty(); }
	/**
	 @}
	 */

	double get_visit_count() { return visitCount; }
	double get_win_count() { return winCount; }

	/**
	 * @brief Get the UCT value
	 * Note that this is never called on root -> parent is never nullptr
	 *
	 * @return uct_t UCT value of the node
	 */
	uct_t get_uct();

	/**
	 * @brief Performs the expansion step of the algorithm
	 *
	 * Creates a new child node by selecting the first non-processed move from
	 * the possibleMoves container, and returns it.
	 *
	 * Othwerise, if there are no non-processed moves i.e. node cannot be
	 * further expanded, returns itself.
	 *
	 * @return MCTS_Node& The expanded node
	 */
	MCTS_Node& expand();

	/**
	 * @brief Updates the state of the node in the backpropagation stage
	 *
	 * It calls the same method for the parent object for every non-root node.
	 * This method increases the winCount by 1 if the player whose move is
	 * connected to this node has won the game. Additionally, calculates the
	 * difference between the winner player's score and the second player's
	 * score and it increases the winCount by 1 for every threshold reached in
	 * the [5, 10, 15, 20] array.
	 *
	 * @param scores The results of a game
	 */
	void update(eval_state_c& scores);

	/**
	 * @brief Clones the MCTSNode::game object and calls the configured
	 * simulation players to make moves until the game ends.
	 *
	 * @return game_t The already ended game that was cloned from the original
	 * game state represented by this node.
	 */
	game_t simulate_game();

	/**
	 * @brief Get the most visited move
	 *
	 * This function is called when the overall performance is evaluated.
	 *
	 * @return move_t
	 */
	move_t get_most_visited_move();

  private:
	game_t                           game;
	const MCTSPlayerContext&         playerCtx;
	std::unique_ptr<move_by_index_c> possibleMoves;
	std::vector<mcts_node_t>         children;
	MCTS_Node*                       parent;
	double                           visitCount;
	double                           winCount;

	/**
	 * @brief Adds the filtered moves to the possibleMoves container
	 *
	 * This method uses the HeuristicFilter and HeuristicCMP functionalities.
	 */
	void add_filtered_moves();

	/**
	 * @brief Updates the winCount the way described in the update(scores)
	 * function This function is only called if the player belonging to this
	 * node has won the game.
	 *
	 * @param scores
	 */
	void update_win_count(eval_state_c& scores);
};

#endif // MCTS_NODE_H