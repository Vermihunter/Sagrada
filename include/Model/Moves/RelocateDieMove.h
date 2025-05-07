#ifndef RELOCATE_DIE_MOVE_H
#define RELOCATE_DIE_MOVE_H

#include "DieToFieldMoveMode.h"
#include "ToolCardMove.h"

/**
 * @brief A specific tool card move that moves a die from a position to another
 * one. It is used by multiple tool cards.
 */
struct RelocateDieMove : ToolCardMove {
	RelocateDieMove(ID_t _playerId, size_t _ind, const BoardIndex& _from, const BoardIndex& _to,
	                dtfm_mode_mask_t _modeMask = 0)
	    : ToolCardMove(_playerId, _ind), from(_from), to(_to), mode(_modeMask)
	{
	}

	virtual void print(std::ostream& os) const override
	{
		os << "Relocating Tool Card Move: [PlayerID: " << playerId << ", from: " << from
		   << ", to: " << to << "]";
	}

	virtual move_t clone(const DiceSupply& from, const DiceSupply& to) override
	{
		return new RelocateDieMove(*this);
	}

	virtual bool contains_randomness() const override { return false; }
	virtual bool is_relocating() const override { return true; }

	Die* die_on_from_field(Game& g);

	virtual bool equals(const move_t& otherMove) override
	{
		RelocateDieMove* otherRelocatingMove = dynamic_cast<RelocateDieMove*>(otherMove);
		return (otherRelocatingMove != nullptr ? equals_to_relocate_move(*otherRelocatingMove)
		                                       : false);
	}

	bool equals_to_relocate_move(const RelocateDieMove& otherRelocateMove)
	{
		return from == otherRelocateMove.from && to == otherRelocateMove.to &&
		       mode == otherRelocateMove.mode;
	}

	BoardIndex       from;
	BoardIndex       to;
	dtfm_mode_mask_t mode;
};

#endif // RELOCATE_DIE_MOVE_H