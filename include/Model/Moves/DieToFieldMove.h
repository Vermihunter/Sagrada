#ifndef DIE_TO_FIELD_MOVE_H
#define DIE_TO_FIELD_MOVE_H

#include "DiceSupply.h"
#include "DieToFieldMoveMode.h"
#include "Move.h"

/**
 * @brief A move that places a die on the field
 *
 * There is a wrapper used around tool cards that have placing a die to a field
 * as a submove called @class PlacingDieMove
 */
struct DieToFieldMove : Move {
	DieToFieldMove(ID_t _playerId, Die* _die, const BoardIndex& _boardIndices,
	               dtfm_mode_mask_t _modeMask)
	    : Move(type::DIE_TO_FIELD_MOVE, _playerId), die(_die), boardIndices(_boardIndices),
	      mode(_modeMask)
	{
	}

	static die_to_field_move_t cast(move_t m) { return (DieToFieldMove*)(m); }

	static die_to_field_move_t create(ID_t playerId, Die* die, const BoardIndex& boardIndices,
	                                  dtfm_mode_mask_t mode = DTFM_MODE_DEFAULT)
	{
		return new DieToFieldMove(playerId, die, boardIndices, mode);
	}

	virtual bool contains_randomness() const override { return false; }
	virtual bool is_relocating() const override { return false; }

	virtual die_to_field_move_t get_placing_submove() override { return this; }

	virtual move_t clone(const DiceSupply& from, const DiceSupply& to) override
	{
		return DieToFieldMove::create(playerId, get_corresponding_die(die, from, to), boardIndices,
		                              mode);
	}

	virtual void print(std::ostream& os) const override
	{
		os << "DieToFieldMove: [playerID: " << playerId << ", to: " << boardIndices << ", " << die
		   << ", ";

		print_mode(os, mode) << "]";
	}

	virtual bool equals(const move_t& otherMove) override
	{
		die_to_field_move_t otherDtfm = dynamic_cast<DieToFieldMove*>(otherMove);
		return (otherDtfm != nullptr ? equals_to_dtfm(*otherDtfm) : false);
	}

	bool equals_to_dtfm(const DieToFieldMove& otherDtfm)
	{
		return *die == *otherDtfm.die && boardIndices == otherDtfm.boardIndices &&
		       mode == otherDtfm.mode;
	}

	Die*             die;
	BoardIndex       boardIndices;
	dtfm_mode_mask_t mode;
};

#endif // DIE_TO_FIELD_MOVE_H