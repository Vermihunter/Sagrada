#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include "BasicMoveHandler.h"
#include "DiceSupply.h"
#include "GameState.h"
#include "IEasyClonable.h"
#include "PlayerInformation.h"
#include "PrivateObjectiveCard.h"
#include "PublicObjectiveCard.h"
#include "RandomGenerator.h"
#include "RoundTrack.h"
#include "ScoreContext.h"
#include "ToolCard.h"
#include "Typedefs.h"

#include <array>
#include <memory>
#include <unordered_map>

class GameContextBuilder;

/**
 * @brief A collection of non-changing data across the game
 *
 * Defines a private constructor and gives access only to the GameContextBuilder
 * class. This forces the users to use the GameContextBuilder class to build
 * objects and that way the GameContext objects will be configured in any case
 * suitably for the game. Doing this makes easier to trust GameContext objects
 * and don't have to check GameContext objects for correctness before or after
 * passing to the Game.
 */
struct GameContext {
	/**
	 * @brief Public copy constructor with concrete random configuration
	 */
	GameContext(const GameContext& other, rnd_t rnd);

	auto clone(rnd_t rnd) { return std::make_unique<GameContext>(*this, std::move(rnd)); }

	wpc_c       selectableWPC;
	puoc_c      publicObjectiveCards;
	tc_c        toolCards;
	player_c    players;
	size_t      numberOfRounds;
	size_t      dicePerRound;
	size_t      playerWpcChoiceCount;
	rnd_t       rnd;
	supply_t    supply;
	RoundTrack  roundTrack;
	score_ctx_t scoreCtx;

	std::unique_ptr<BasicMoveHandler> basicMoveHandler;

  private:
	friend GameContextBuilder;
	GameContext() {}
};

using game_context_t = std::unique_ptr<GameContext>;

#endif // GAME_CONTEXT_H
