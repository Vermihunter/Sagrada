#ifndef REROLL_ALL_DICE_TC_H
#define REROLL_ALL_DICE_TC_H

#include <vector>

#include "Die.h"
#include "RandomGenerator.h"
#include "ToolCard.h"

/**
 * @brief Implementation of the Glazing Hammer Tool card
 *
 * Rerolls all the remaining dice from the supply. This tool card
 * could be used only in the player's second turn of any round.
 */
class RerollAllDiceTC : public ToolCard
{
  public:
    /** Default constructor */
    RerollAllDiceTC();
    /** Default destructor */
    virtual ~RerollAllDiceTC();

    virtual ToolCardUseFeedback use(tool_card_move_t m, Game& g) override;

    virtual void append_possible_moves(ToolCardAppendContext& ctx) override;

    virtual tc_t clone() override
    {
        return std::make_unique<RerollAllDiceTC>(*this);
    }

    virtual ToolCardUseFeedback undo(tool_card_move_t& m, Game& g) override;

    struct Move : ToolCardMove
    {
        Move(ID_t _playerId, size_t _ind) : ToolCardMove(_playerId, _ind) {}

        virtual void print(std::ostream& os) const override
        {
            os << "RerollAllDieToolCardMove: [playerID: " << playerId << "]";
        }

        virtual move_t clone(const DiceSupply& from,
                             const DiceSupply& to) override
        {
            auto move = new RerollAllDiceTC::Move(playerId, ind);
            for (auto&& [die, originalValue] : originalValues) {
                move->originalValues.push_back(
                    {get_corresponding_die(die, from, to), originalValue});
            }
            return move;
        }
        virtual bool contains_randomness() const override { return true; }
        virtual bool is_relocating() const override { return false; }

        virtual bool equals(const move_t& otherMove) override
        {
            RerollAllDiceTC::Move* otherRerollingMove =
                dynamic_cast<RerollAllDiceTC::Move*>(otherMove);
            if (!otherRerollingMove ||
                originalValues.size() !=
                    otherRerollingMove->originalValues.size()) {
                return false;
            }

            for (size_t i = 0; i < originalValues.size(); ++i) {
                if (*originalValues[i].first !=
                        *otherRerollingMove->originalValues[i].first ||
                    originalValues[i].second !=
                        otherRerollingMove->originalValues[i].second) {
                    return false;
                }
            }

            return true;
        }

        std::vector<std::pair<Die*, value_t>> originalValues;
    };
};

#endif // REROLL_ALL_DICE_TC_H
