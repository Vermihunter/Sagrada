#ifndef CHANGE_DIE_VALUE_TC_H
#define CHANGE_DIE_VALUE_TC_H

#include "BoardIndex.h"
#include "Die.h"
#include "DieToFieldMove.h"
#include "ToolCard.h"

/**
 * @brief Implementation of the Grozing Pliers tool card
 *
 * The player chooses a die, increments/decrements its value by 1 if possible
 * and places somewhere on his board.
 *
 * If a die would be not placeable after the value increment/decrement, it is
 * not possible to modify the value and cannot use this move.
 */
class ChangeDieValueTC : public ToolCard {
  public:
	/** Default constructor */
	ChangeDieValueTC();
	/** Default destructor */
	virtual ~ChangeDieValueTC();

	/** Called when a part of the tool card's move is requested to be made */
	virtual ToolCardUseFeedback use(tool_card_move_t m, Game& g) override;

	/** Appends moves to the container which the AI player will choose from */
	virtual void append_possible_moves(ToolCardAppendContext& ctx) override;

	virtual tc_t clone() override { return std::make_unique<ChangeDieValueTC>(*this); }

	/** Undo operation - Currently unused */
	virtual ToolCardUseFeedback undo(tool_card_move_t& m, Game& g) override;

	/** Part of the public API so the View component could save the moves by
	 * value changers */
	void process_hypothetical_die_with_val(ToolCardAppendContext& ctx, Die* die, int changeVal);

	struct Move : ToolCardMove {
		Move(ID_t _playerId, size_t _ind, Die* _die, int _changeVal, const BoardIndex& boardInd)
		    : ToolCardMove(_playerId, _ind), dtfm(_playerId, _die, boardInd, DTFM_MODE_DEFAULT),
		      changeVal(_changeVal)
		{
		}

		virtual void print(std::ostream& os) const override
		{
			os << "ChangeDieValueToolCardMove: ["
			      "playerID: "
			   << playerId << ", change value: " << changeVal << ", dtfm: ";
			dtfm.print(os);
			os << "]";
		}

		virtual move_t clone(const DiceSupply& from, const DiceSupply& to) override
		{
			return new ChangeDieValueTC::Move(playerId, ind,
			                                  get_corresponding_die(dtfm.die, from, to), changeVal,
			                                  dtfm.boardIndices);
		}

		static tool_card_move_t from_context(ToolCardAppendContext& ctx, Die* die, int changeVal,
		                                     const BoardIndex& ind);

		virtual die_to_field_move_t get_placing_submove() override { return &dtfm; }

		virtual bool contains_randomness() const override { return false; }
		virtual bool is_relocating() const override { return false; }

		virtual bool equals(const move_t& otherMove) override
		{
			ChangeDieValueTC::Move* otherTCMove = dynamic_cast<ChangeDieValueTC::Move*>(otherMove);
			if (!otherTCMove) {
				return false;
			}

			return dtfm.equals_to_dtfm(otherTCMove->dtfm) && changeVal == otherTCMove->changeVal;
		}

		DieToFieldMove dtfm;
		int            changeVal;
	};

	void modify_die_value(Die* die, int value);

  private:
	using this_move_t = ChangeDieValueTC::Move*;

	/** Use implementation */
	void append_moves_for_die(ToolCardAppendContext& ctx, Die* die, int changeVal);

	/** Used for undo as well */
	void modify_value_move_part(this_move_t& m, int byVal);
};

#endif // CHANGE_DIE_VALUE_TC_H
