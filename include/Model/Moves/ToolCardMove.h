#ifndef TOOL_CARd_MOVE_H
#define TOOL_CARd_MOVE_H

#include "DiceSupply.h"
#include "Move.h"

struct ToolCardMultipleMoveInformation {
	bool isSingleMoveToolCard;
	bool followUpMoveExists;
};

/**
 * @brief Base class for each Tool card move
 *
 * Each Tool card makes a derived struct from this Move so the game
 * knew it is a Tool card move and has to be checked and evaluated
 * differently.
 */
struct ToolCardMove : Move {
	ToolCardMove(ID_t _playerId, size_t _ind) : Move(type::TOOL_CARD, _playerId), ind(_ind) {}
	virtual ~ToolCardMove() = default;

	virtual ToolCardMultipleMoveInformation consists_of_multiple_submoves()
	{
		return ToolCardMultipleMoveInformation{.isSingleMoveToolCard = true,
		                                       .followUpMoveExists   = false};
	}

	size_t ind;
};

#endif // TOOL_CARd_MOVE_H