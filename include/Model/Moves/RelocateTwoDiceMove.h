#ifndef RELOCATE_TWO_DICE_MOVE_H
#define RELOCATE_TWO_DICE_MOVE_H

#include "RelocateDieMove.h"
#include "Typedefs.h"

#include <memory>

/**
 * @brief A tool card move that reloces two dice
 * It is used by two Tool cards currently: RelocateExactlyTwoDiceTC and
 * RelocateMaxTwoSameColorDiceTC
 */
struct RelocateTwoDiceMove : ToolCardMove {
	RelocateTwoDiceMove(const RelocateDieMove& _m1, const RelocateDieMove& _m2)
	    : ToolCardMove(_m1.playerId, _m1.ind), m1(_m1), m2(_m2), hasSecondRelocateMove(true)
	{
	}

	RelocateTwoDiceMove(const RelocateDieMove& singleMove)
	    : ToolCardMove(singleMove.playerId, singleMove.ind), m1(singleMove), m2(singleMove),
	      hasSecondRelocateMove(false)
	{
	}

	~RelocateTwoDiceMove() = default;

	virtual void print(std::ostream& os) const override
	{
		os << "RelocateTwoDiceToolCardMove: [move1: ";
		m1.print(os);

		if (hasSecondRelocateMove) {
			os << ", move2: ";
			m2.print(os);
		}

		os << "]";
	}

	virtual move_t clone(const DiceSupply& from, const DiceSupply& to) override
	{
		return new RelocateTwoDiceMove(*this);
	}

	virtual bool contains_randomness() const override { return false; }
	virtual bool is_relocating() const override { return true; }

	virtual bool equals(const move_t& otherMove) override
	{
		RelocateTwoDiceMove* otherRelocatingMove = dynamic_cast<RelocateTwoDiceMove*>(otherMove);
		if (!otherRelocatingMove) {
			return false;
		}

		return m1.equals_to_relocate_move(otherRelocatingMove->m1) &&
		       ((!hasSecondRelocateMove && !otherRelocatingMove->hasSecondRelocateMove) ||
		        (hasSecondRelocateMove && otherRelocatingMove->hasSecondRelocateMove &&
		         m2.equals_to_relocate_move(otherRelocatingMove->m2)));
	}

	RelocateDieMove m1;
	RelocateDieMove m2;
	bool            hasSecondRelocateMove;
};

using relocate_two_dice_move_t = RelocateTwoDiceMove*;

#endif // RELOCATE_TWO_DICE_MOVE_H