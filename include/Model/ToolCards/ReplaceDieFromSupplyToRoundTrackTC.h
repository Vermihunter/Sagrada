#ifndef REPLACE_DIE_FROM_SUPPLY_TO_ROUNDTRACK_TC_H
#define REPLACE_DIE_FROM_SUPPLY_TO_ROUNDTRACK_TC_H

#include "Die.h"
#include "DieToFieldMove.h"
#include "ToolCard.h"

/**
 * @brief Implementation of the Lens Cutter Tool card
 *
 * Chooses a die from the supply, replaces with one from the round track
 * and places the die on a field obeying all placement restrictions.
 * If the die from the round track is not placeable to any field, it is not
 * possible to swap it with the selected one from the supply.
 */
class ReplaceDieFromSupplyToRoundTrackTC : public ToolCard
{
  public:
    /** Default constructor */
    ReplaceDieFromSupplyToRoundTrackTC();
    /** Default destructor */
    virtual ~ReplaceDieFromSupplyToRoundTrackTC();

    virtual ToolCardUseFeedback use(tool_card_move_t m, Game& g) override;

    virtual void append_possible_moves(ToolCardAppendContext& ctx) override;

    virtual tc_t clone() override
    {
        return std::make_unique<ReplaceDieFromSupplyToRoundTrackTC>(*this);
    }

    virtual ToolCardUseFeedback undo(tool_card_move_t& m, Game& g) override;

    struct Move : ToolCardMove
    {
        Move(ID_t _playerId, size_t _ind, Die* dieToReplace,
             Die* dieToReplaceWith, const DieToFieldMove& move)
            : ToolCardMove(_playerId, _ind)
            , dieInSupply(dieToReplace)
            , dieOnRoundTrack(dieToReplaceWith)
            , placingMove(move)
        {
        }

        virtual void print(std::ostream& os) const override
        {
            os << "ReplaceDieFromSupplyToRoundTrackToolCardMove: [playerID: "
               << playerId << ", die from supply: " << dieInSupply
               << ", die on round track: " << dieOnRoundTrack << "] place to "
               << placingMove.boardIndices;
        }

        virtual move_t clone(const DiceSupply& from,
                             const DiceSupply& to) override
        {
            auto mv = new ReplaceDieFromSupplyToRoundTrackTC::Move(
                playerId, ind, nullptr, nullptr,
                DieToFieldMove {
                    playerId, get_corresponding_die(placingMove.die, from, to),
                    placingMove.boardIndices, placingMove.mode});
            mv->dieInSupply = get_corresponding_die(dieInSupply, from, to);
            mv->dieOnRoundTrack =
                get_corresponding_die(dieOnRoundTrack, from, to);
            return mv;
        }

        virtual die_to_field_move_t get_placing_submove() override
        {
            return &placingMove;
        }

        virtual bool contains_randomness() const override { return false; }
        virtual bool is_relocating() const override { return false; }

        virtual bool equals(const move_t& otherMove) override
        {
            ReplaceDieFromSupplyToRoundTrackTC::Move* otherReplacingMove =
                dynamic_cast<ReplaceDieFromSupplyToRoundTrackTC::Move*>(
                    otherMove);
            if (!otherReplacingMove) {
                return false;
            }

            return *dieInSupply == *otherReplacingMove->dieInSupply &&
                   *dieOnRoundTrack == *otherReplacingMove->dieOnRoundTrack &&
                   placingMove.equals_to_dtfm(otherReplacingMove->placingMove);
        }

        Die* dieInSupply;
        Die* dieOnRoundTrack;
        DieToFieldMove placingMove;
    };

  private:
    using replace_move_t = ReplaceDieFromSupplyToRoundTrackTC::Move*;

    void replace_dice(replace_move_t& mv, Game& g);
    void append_moves_for(Die* currentRoundDie, Die* roundTrackDie,
                          ToolCardAppendContext& ctx);
};

#endif // REPLACE_DIE_FROM_SUPPLY_TO_ROUNDTRACK_TC_H
