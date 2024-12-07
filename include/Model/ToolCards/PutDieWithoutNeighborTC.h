#ifndef PUTDIEWITHOUTNEIGHBORTC_H
#define PUTDIEWITHOUTNEIGHBORTC_H

#include "Die.h"
#include "DieToFieldMove.h"
#include "RandomGenerator.h"
#include "ToolCard.h"

/**
 * @brief Implementation of the Cork-backed Straightedge Tool card
 *
 * Using this tool card a user can place a die to a field that is not reachable.
 * If the field is reachable, this tool card cannot be used.
 */
class PutDieWithoutNeighborTC : public ToolCard
{
  public:
    /** Default constructor */
    PutDieWithoutNeighborTC();
    /** Default destructor */
    virtual ~PutDieWithoutNeighborTC();

    virtual ToolCardUseFeedback use(tool_card_move_t m, Game& g) override;

    virtual void append_possible_moves(ToolCardAppendContext& ctx) override;

    virtual tc_t clone() override
    {
        return std::make_unique<PutDieWithoutNeighborTC>(*this);
    }

    virtual ToolCardUseFeedback undo(tool_card_move_t& m, Game& g) override;

    struct Move : ToolCardMove
    {
        Move(ID_t _playerId, size_t _ind, const DieToFieldMove& _bm)
            : ToolCardMove(_playerId, _ind), bm(_bm)
        {
        }

        virtual void print(std::ostream& os) const override
        {
            os << "PutDieWithoutNeighborToolCardMove:";
            bm.print(os);
        }

        virtual move_t clone(const DiceSupply& from,
                             const DiceSupply& to) override
        {
            return new PutDieWithoutNeighborTC::Move(
                playerId, ind,
                DieToFieldMove {playerId,
                                get_corresponding_die(bm.die, from, to),
                                bm.boardIndices, bm.mode});
        }

        virtual die_to_field_move_t get_placing_submove() override
        {
            return &bm;
        }

        virtual bool contains_randomness() const override { return false; }
        virtual bool is_relocating() const override { return false; }

        virtual bool equals(const move_t& otherMove) override
        {
            PutDieWithoutNeighborTC::Move* otherPutDieWithoutNeighborMove =
                dynamic_cast<PutDieWithoutNeighborTC::Move*>(otherMove);
            return (otherPutDieWithoutNeighborMove != nullptr
                        ? bm.equals_to_dtfm(otherPutDieWithoutNeighborMove->bm)
                        : false);
        }

        DieToFieldMove bm;
    };
};

#endif // PUTDIEWITHOUTNEIGHBORTC_H
