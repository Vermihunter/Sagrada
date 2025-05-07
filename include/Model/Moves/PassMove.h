#ifndef PASS_MOVE_H
#define PASS_MOVE_H

#include "Move.h"

/**
 * @brief A type of move that doesn't have any effect to the player's board.
 * This move shifts the round.
 */
struct PassMove : Move {
	PassMove(ID_t _playerId) : Move(type::PASS_MOVE, _playerId) {}

	virtual move_t clone(const DiceSupply& from, const DiceSupply& to) override
	{
		return new PassMove(*this);
	}

	virtual bool contains_randomness() const override { return false; }
	virtual bool is_relocating() const override { return false; }

	virtual bool equals(const move_t& otherMove) override
	{
		return dynamic_cast<PassMove*>(otherMove) == nullptr;
	}

	virtual void print(std::ostream& os) const override
	{
		os << "PassMove: [playerID: " << playerId << "]";
	}

	static pass_move_t create(ID_t _playerId) { return new PassMove(_playerId); }
};

#endif // PASS_MOVE_H