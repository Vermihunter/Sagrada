#ifndef OVERALL_BOARD_STATE_H
#define OVERALL_BOARD_STATE_H

#include "BasicMoveHandler.h"
#include "Board.h"

class Game;

/**
 * @brief Represents a board's global state
 *
 */
struct OverallBoardState
{
    OverallBoardState(const Game& g, const ID_t playerID);

    std::vector<BoardIndex> unfillableFieldIndices;
    size_t uncompletableFieldCount;

  private:
    void add_unfillable_field_indices(const Game& g, const ID_t playerID);
};

#endif // OVERALL_BOARD_STATE_H