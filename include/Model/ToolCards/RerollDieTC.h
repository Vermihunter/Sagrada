#ifndef REROLL_DIE_TC_H
#define REROLL_DIE_TC_H

#include "Die.h"
#include "PlacingDieMove.h"
#include "RandomGenerator.h"
#include "ToolCard.h"

/**
 * @brief Implementation of the Flux Brush Tool card
 *
 * Selects a die and rerolls it. If the die with the new value
 * is placeable to a field, the player must place it somewhere.
 * Otherwise the die has to be returned to the Draf pool.
 */
class RerollDieTC : public ToolCard {
  public:
	/** Default constructor */
	RerollDieTC();
	/** Default destructor */
	virtual ~RerollDieTC();

	virtual ToolCardUseFeedback use(tool_card_move_t m, Game& g) override;

	virtual void append_possible_moves(ToolCardAppendContext& ctx) override;

	virtual std::unique_ptr<ToolCard> clone() override
	{
		return std::make_unique<RerollDieTC>(*this);
	}

	virtual ToolCardUseFeedback undo(tool_card_move_t& m, Game& g) override;

	struct Move : ToolCardMove {
		Move(ID_t _playerId, size_t _ind, Die* d, value_t _prevVal, bool _wasPlaceable = false)
		    : ToolCardMove(_playerId, _ind), die(d), prevVal(_prevVal), wasPlaceable(_wasPlaceable)
		{
		}

		virtual ToolCardMultipleMoveInformation consists_of_multiple_submoves() override
		{
			return ToolCardMultipleMoveInformation{.isSingleMoveToolCard = false,
			                                       .followUpMoveExists   = wasPlaceable};
		}

		virtual void print(std::ostream& os) const override
		{
			os << "RerollDieToolCardMove: [playerID: " << playerId
			   << ", original value: " << size_t(prevVal)
			   << ", rerolled value: " << size_t(die->get_value()) << "]";
		}

		virtual move_t clone(const DiceSupply& from, const DiceSupply& to) override
		{
			return new RerollDieTC::Move(playerId, ind, get_corresponding_die(die, from, to),
			                             prevVal, wasPlaceable);
		}

		virtual bool contains_randomness() const override { return true; }
		virtual bool is_relocating() const override { return false; }

		virtual bool equals(const move_t& otherMove) override
		{
			RerollDieTC::Move* otherRerollingMove = dynamic_cast<RerollDieTC::Move*>(otherMove);
			if (!otherRerollingMove) {
				return false;
			}

			return *die == *otherRerollingMove->die && prevVal == otherRerollingMove->prevVal &&
			       wasPlaceable == otherRerollingMove->wasPlaceable;
		}

		Die* die;
		/** Needed for undo operation */
		value_t prevVal;
		bool    wasPlaceable;
	};

  private:
	using reroll_move_t = RerollDieTC::Move*;

	bool move_exists(reroll_move_t& rerollSubmove, Game& g);
	void append_reroll_moves(ToolCardAppendContext& ctx);
	void append_die_placing_moves(ToolCardAppendContext& ctx);
};

#endif // REROLL_DIE_TC_H
