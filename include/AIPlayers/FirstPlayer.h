#ifndef FIRST_PLAYER_H
#define FIRST_PLAYER_H

#include <iostream>
#include <memory>

#include "AI_Player.h"
#include "Move.h"
#include "Typedefs.h"

/**
 * @brief An AI player choosing the first possible move
 *
 * This player does not get all the possible moves from the
 * Game::possible_moves() function but has a method specifically designed for
 * this player to have a great performance that collects always a single move.
 */
class FirstPlayer : public AI_Player
{
  public:
    FirstPlayer(const AIPlayerContext& _ctx);

    /** Default destructor */
    virtual ~FirstPlayer();

    virtual ID_t choose_window_pattern_card(wpc_it_t iBegin,
                                            wpc_it_t iEnd) override;

    virtual move_t make_next_move() override;
};

#endif // FIRST_PLAYER_H
