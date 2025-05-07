#ifndef AI_PLAYER_H
#define AI_PLAYER_H

#include "AI_PlayerConfig.h"
#include "AI_PlayerContext.h"
#include "Game.h"
#include "Move.h"
#include "PrivateObjectiveCard.h"
#include "PublicObjectiveCard.h"
#include "ToolCard.h"
#include "Typedefs.h"

#include <array>
#include <chrono>
#include <memory>

/**
 * @brief Base class for AI players
 */
class AI_Player {
  protected:
	using wpc_it_t = wpc_c::iterator;

  public:
	AI_Player(const AIPlayerContext&);

	virtual ~AI_Player() = default;

	/**
	 * Returns relative index from the begin iterator it gets,
	 * e.g. returns 0 if it chose the WPC that iBegin was pointing to.
	 */
	virtual ID_t choose_window_pattern_card(wpc_it_t iBegin, wpc_it_t iEnd) = 0;

	/**
	 * The concrete AI player will define the algorithm it chooses
	 * to make a move.
	 */
	virtual move_t make_next_move() = 0;

	/** Used when calling all AI players until its the human player's turn */
	auto get_id() { return ctx.ID; }

	static ai_player_t initialize_player(Game& game, const AI_PlayerConfig& config,
	                                     const player_t& playerInfo, const bool gameDeterministic);

	/** Used for statistics - some players don't operate on all possible moves
	 * (Rules-based only DTFM) */
	virtual move_c get_possible_moves(Game& g) { return g.possible_moves(); }

  protected:
	AIPlayerContext ctx;
};

#endif // AI_PLAYER_H
