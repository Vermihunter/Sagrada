#ifndef MCTS_PLAYER_H
#define MCTS_PLAYER_H

#include "AI_Player.h"
#include "BoardFieldNeighborCache.h"
#include "Deallocator.h"
#include "HeuristicFilter.h"
#include "MCTSNode.h"
#include "MCTSPlayerContext.h"
#include "MoveHeuristicCMP.h"
#include "RelocateDieMove.h"
#include "RelocateTwoDiceMove.h"
#include "Typedefs.h"

#include <algorithm>
#include <iostream>
#include <memory>

struct MCTSPlayerConfig;

/**
 * @brief Represents an AI player that uses the Monte Carlo Tree Search
 * algorithm to perform moves. For deterministic games, it simply runs the
 * number of iterations in the deterministic mode of the game.
 *
 * In non-deterministic games, it creates determinizing worlds and runs
 * TOTAL_IT_COUNT / WORLD_COUNT in each world. After that it combines the
 * results and chooses the one that produced the best results in average.
 *
 * Uses the MCTSNode class for representing the nodes.
 */
class MCTSPlayer : public AI_Player {
  public:
	MCTSPlayer(const AIPlayerContext& _ctx, mcts_context_t _playerCtx);
	virtual ~MCTSPlayer();

	virtual ID_t choose_window_pattern_card(wpc_it_t iBegin, wpc_it_t iEnd) override;

	/**
	 * @brief Implements the MCTS algorithm
	 *
	 * @return move_t
	 */
	virtual move_t make_next_move();

  private:
	const mcts_context_t playerCtx;
	ai_player_config_c   playoutStrategies;

	/**
	 * @brief Runs the algorithm in a single world
	 * In the deterministic world, it creates the node that will hold the best
	 * move.
	 *
	 * In the non-deterministic world, produces a root for a determinizing
	 * world.
	 *
	 * @param gameWorld
	 * @return std::unique_ptr<MCTS_Node>
	 */
	std::unique_ptr<MCTS_Node> get_root_from_world(game_t gameWorld);

	/**
	 * @brief Creates the worlds for the determinizing technique.
	 * It handles the randomness so the different worlds had different random
	 * configurations.
	 *
	 * @param worldCount Number of random worlds to create
	 * @return std::vector<game_t> The random worlds
	 */
	std::vector<game_t> create_worlds(int worldCount);

	/**
	 * @brief Provides the selection phase of the MCTS algorithm
	 * Chooses the node with the maximum MCTS_Node::get_uct() method to traverse
	 * to the next layer.
	 *
	 * @param _root
	 * @return MCTS_Node&
	 */
	MCTS_Node& select(MCTS_Node& _root);
};

#endif // MCTS_PLAYER_H
