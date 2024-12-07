#ifndef MOVE_TWICE_IN_A_ROW_TC_H
#define MOVE_TWICE_IN_A_ROW_TC_H

#include "GameHistory.h"
#include "Move.h"
#include "ToolCard.h"

struct TurnInfo;

/**
 * @brief Implementation of the Running Pliers tool card
 *
 * The current user can use this tool card to move twice in a row.
 * If a player would move twice in a row anyways, this tool card cannot be used.
 * Using this tool card doesn't shift the round so after this move
 * the current player has still two moves left.
 */
class MoveTwiceInARowTC : public ToolCard
{
  public:
    /** Default constructor */
    MoveTwiceInARowTC();
    /** Default destructor */
    virtual ~MoveTwiceInARowTC();

    virtual ToolCardUseFeedback use(tool_card_move_t m, Game& g) override;

    virtual void append_possible_moves(ToolCardAppendContext& ctx) override;

    virtual tc_t clone() override
    {
        return std::make_unique<MoveTwiceInARowTC>(*this);
    }

    virtual ToolCardUseFeedback undo(tool_card_move_t& m, Game& g) override;

    struct Move : ToolCardMove
    {
        Move(ID_t _playerId, size_t _ind) : ToolCardMove(_playerId, _ind) {}

        virtual void print(std::ostream& os) const override
        {
            os << "MoveTwiceInARowToolCardMove: [playerID: " << playerId << "]";
        }

        virtual move_t clone(const DiceSupply& from,
                             const DiceSupply& to) override
        {
            return new MoveTwiceInARowTC::Move(*this);
        }

        virtual bool contains_randomness() const override { return false; }
        virtual bool is_relocating() const override { return false; }

        virtual bool equals(const move_t& otherMove) override
        {
            return dynamic_cast<MoveTwiceInARowTC::Move*>(otherMove) != nullptr;
        }

        std::vector<TurnInfo> prevRoundInfo;
    };
};

#endif // MOVE_TWICE_IN_A_ROW_TC_H
