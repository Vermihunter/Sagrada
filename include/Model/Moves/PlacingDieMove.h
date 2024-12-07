#ifndef PLACING_DIE_MOVE_H
#define PLACING_DIE_MOVE_H

#include "DieToFieldMove.h"
#include "ToolCardMove.h"

/**
 * @brief Wrapper for die to field move for tool cards
 *
 * Used in the last phase of given tool cards when a user manipulates
 * a die (e.g. increases/decreases its value by 1) and then has to place
 * that die to the board.
 */
struct PlacingDieMove : ToolCardMove
{
    PlacingDieMove(ID_t _playerId, size_t _ind, const DieToFieldMove& _bm)
        : ToolCardMove(_playerId, _ind), bm(_bm)
    {
    }

    ~PlacingDieMove()
    {
        // delete bm;
    }

    static auto create(ID_t _playerId, size_t _ind, DieToFieldMove _bm)
    {
        return new PlacingDieMove(_playerId, _ind, _bm);
    }

    virtual move_t clone(const DiceSupply& from, const DiceSupply& to) override
    {
        return new PlacingDieMove(
            playerId, ind,
            DieToFieldMove {bm.playerId,
                            get_corresponding_die(bm.die, from, to),
                            bm.boardIndices, bm.mode});
    }

    virtual die_to_field_move_t get_placing_submove() override { return &bm; }

    virtual void print(std::ostream& os) const override
    {
        os << "Tool card placing move: ";
        bm.print(os);
    }

    virtual bool equals(const move_t& otherMove) override
    {
        PlacingDieMove* otherPlacingDieMove =
            dynamic_cast<PlacingDieMove*>(otherMove);
        return (otherPlacingDieMove != nullptr
                    ? bm.equals_to_dtfm(otherPlacingDieMove->bm)
                    : false);
    }

    virtual bool contains_randomness() const override { return false; }
    virtual bool is_relocating() const override { return false; }

    // die_to_field_move_t bm;
    DieToFieldMove bm;
};

#endif // PLACING_DIE_MOVE_H