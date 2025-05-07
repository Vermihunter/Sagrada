#ifndef RANDOM_PLAYER_H
#define RANDOM_PLAYER_H

#include "AI_Player.h"
#include "Move.h"
#include "RandomGenerator.h"
#include "Typedefs.h"

#include <iostream>
#include <memory>

/**
 * @brief An AI player making random moves
 *
 */
class RandomPlayer : public AI_Player {
  public:
	/** Default constructor */
	RandomPlayer(const AIPlayerContext& _ctx);

	/** Default destructor */
	virtual ~RandomPlayer();

	virtual ID_t   choose_window_pattern_card(wpc_it_t iBegin, wpc_it_t iEnd) override;
	virtual move_t make_next_move() override;
};

#endif // RANDOM_PLAYER_H
