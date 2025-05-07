#ifndef AI_PLAYER_CONTEXT_H
#define AI_PLAYER_CONTEXT_H

#include "Game.h"
#include "PrivateObjectiveCard.h"
#include "Typedefs.h"

/**
 * @brief Represents the game specific information that is common for every AI
 * player
 */
struct AIPlayerContext {
	ID_t   ID;
	Game&  game;
	proc_t privateObjectiveCard;
	bool   gameDeterministic;
};

#endif // AI_PLAYER_CONTEXT_H