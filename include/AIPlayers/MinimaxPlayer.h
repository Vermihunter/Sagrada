#ifndef MINIMAX_PLAYER_H
#define MINIMAX_PLAYER_H

#include <algorithm>

#include "AI_Player.h"
#include "Board.h"
#include "HeuristicConstants.h"
#include "MinimaxPlayerType.h"
#include "MoveHeuristicCMP.h"
#include "Typedefs.h"

using heuristic_result_t = int;
const heuristic_result_t HEURISTIC_MIN = MinMaxVal<heuristic_result_t>::min;
const heuristic_result_t HEURISTIC_MAX = MinMaxVal<heuristic_result_t>::max;

/**
 * @brief Represents the Minimax AI player
 *
 */
class MinimaxPlayer : public AI_Player
{
  public:
    MinimaxPlayer(const AIPlayerContext& _ctx, size_t depth,
                  const HeuristicConstants& _constants, int worldCount);

    virtual ID_t choose_window_pattern_card(wpc_it_t iBegin,
                                            wpc_it_t iEnd) override;

    /**
     * @brief Provides the Minimax algorithm for both deterministic and
     * non-deterministic worlds
     *
     * In the deterministic mode, it runs the Minimax algorithm in a single -
     * deterministic world.
     *
     * In the non-deterministic mode, it runs the Minimax algorithm in the
     * number of worlds that is provided in the configuration, merges the
     * results and chooses the one that produced the best results in average
     * across the worlds.
     *
     * @return move_t The chosen move
     */
    virtual move_t make_next_move() override;

  private:
    constexpr static int INITIAL_RANDOM_SEED = 996633210;
    MinimizingPlayer<heuristic_result_t> minimizingPlayer;
    MaximizingPlayer<heuristic_result_t> maximizingPlayer;

    size_t depth;
    HeuristicConstants constants;
    int worldCount;

    /**
     * @brief Heuristic function that evaluates a current state from the
     * perspective of the Minimax player It evaluates every player's board and
     * merges the results. The opponent players' results are multiplied with a
     * weight.
     *
     * If the game has already ended, it simply returns a big positive constant
     * for win and a small negative constant for lose. It does not return
     * MinMaxValue<heuristic_result_t>::min/max because in case of defining
     * multiple worlds, merging these subresults would overflow. For this
     * reason, the MinMaxValue<heuristic_result_t>::min/max are divided by a
     * constant.
     *
     * @param gameState
     * @return heuristic_result_t
     */
    heuristic_result_t heuristic(Game& gameState);

    /**
     * @brief Evaluates a single player's state
     * The evaluation includes:
     * - the weighted total score achieved by the time of evaluation
     * - the weighted Public Objective Card completable patterns
     * - the weighted Public Objective Card uncompletable patterns
     * - the weighted uncompletable field count for the board
     *
     * The weights are gained from the HeuristicConstants configuration that is
     * sent to the constructor.
     *
     * @param evalState The state that is being evaluated
     * @return heuristic_result_t The heuristic result of the state
     */
    heuristic_result_t get_heuristic_value_for_score(
        const eval_state_t& evalState);

    /**
     * @brief The recursive implementation of the Minimax algorithm
     *
     * @param g The game state representing the context
     * @param depth The remaining depth of a given call
     * @param alpha Alpha value of the alpha-beta pruning technique
     * @param beta Beta value of the alpha-beta pruning technique
     * @param playerType The minimizing or maximizing player providing the
     * interface
     * @param movesWithHeuristicValue Output for the layer where depth ==
     * MinimaxPlayer::depth - 1 used for the non-deterministic result merging
     * @return std::pair<move_t, heuristic_result_t> The best move-heuristic
     * value pair for the playerType on the given depth
     */
    std::pair<move_t, heuristic_result_t> minimax_algorithm(
        Game& g, size_t depth, int alpha, int beta,
        MinimaxPlayerType<heuristic_result_t>& playerType,
        std::vector<std::pair<move_t, heuristic_result_t>>*
            movesWithHeuristicValue);

    /**
     * @brief Collects the heuristically filtered and sorted possible moves in a
     * given game state
     *
     * @param g The game state representing the context
     * @param allMoves The moves to be filtered and sorted
     * @param bestMoves The moves evaluated to be stronger
     * @param backupMoves The moves evaluated to be weaker
     * @return move_by_index_c& The reference for the container to use -
     * bestMoves if non-empty, backupMoves if bestMoves is empty
     */
    move_by_index_c& get_moves(Game& g, move_c& allMoves,
                               move_by_index_c& bestMoves,
                               move_by_index_c& backupMoves);
};

#endif // MINIMAX_PLAYER_H