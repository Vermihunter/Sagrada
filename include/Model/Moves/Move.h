#ifndef MOVE_H
#define MOVE_H

#include "BoardIndex.h"
#include "Die.h"
#include "Typedefs.h"

#include <exception>
#include <iostream>

class Game;

/**
 * @brief Exception thrown by Game when a move request is incorrect
 *
 * Makes the communication between components easier that a failed
 * move request can tell the reason why it failed and doesn't have to
 * check in each component for correctness.
 */
class BadMoveRequest : public std::exception {
  public:
	BadMoveRequest(const std::string& _msg) : msg(_msg) {}

	const char* what() const noexcept override { return msg.c_str(); }

  private:
	std::string msg;
};

/**
 * @brief Base class for moves
 *
 * A move objects represents moves that players send to the game component.
 *
 */
struct Move {
	virtual ~Move() = default;

	enum class type { DIE_TO_FIELD_MOVE, TOOL_CARD, PASS_MOVE };

	Move(type _t, ID_t _playerId) : t(_t), playerId(_playerId) {}

	/** Interface for debugging */
	virtual void print(std::ostream& os) const = 0;

	/** Clones a move from a game to a cloned game object since not all moves
	 * are compatible with cloned games */
	virtual move_t clone(const DiceSupply& from, const DiceSupply& to) = 0;

	/** Help for filtering the tool card moves to detect if it has a placing
	 * submove */
	virtual die_to_field_move_t get_placing_submove() { return nullptr; }

	/** Interface required for the rules-based player to categorize moves easier
	 */
	virtual bool contains_randomness() const = 0;
	virtual bool is_relocating() const       = 0;

	virtual bool equals(const move_t& otherMove) = 0;

	friend std::ostream& operator<<(std::ostream& os, const move_t& m)
	{
		m->print(os);
		return os;
	}

	type t;
	ID_t playerId;
};

#endif // MOVE_H