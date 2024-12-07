#ifndef BOARD_FIEL_H
#define BOARD_FIEL_H

#include <memory>
#include <optional>

#include "Attributes.h"
#include "BoardFieldRestrictions.h"
#include "BoardIndex.h"
#include "DiceSupply.h"
#include "Die.h"
#include "Move.h"

/**
 * @brief Represents a field on the board.
 *
 * The fields are constructed from the input WPC. They
 * can have at most 1 restriction. A die could be placed
 * to a boardfield if it fulfils both value and color
 * restrictions. If an attribute doesn't have
 * a restriction, it is taken as fulfilled.
 */
class BoardField
{
    const restriction_t restrictions;
    Die* die;

  public:
    BoardField();
    BoardField(BoardField&&);

    /** Each board field could have one or zero restrictions */
    BoardField(const restriction_t restrictions);

    BoardField(const BoardField& bf, DiceSupply& newDS, DiceSupply& originalDS);

    static auto create_empty()
    {
        return new BoardField(std::make_unique<BoardFieldRestrictions>());
    }

    auto clone(DiceSupply& newDS, DiceSupply& originalDS)
    {
        return new BoardField(*this, newDS, originalDS);
    }

    auto clone_without_die() const
    {
        auto restr = restrictions->clone();
        return new BoardField(std::move(restr));
    }

    /** For generic processing - e.g. Evaluator */
    template <typename A>
    constexpr auto get_attr_restriction()
    {
        if constexpr (std::is_same_v<A, value_t>)
            return get_value_restriction();
        if constexpr (std::is_same_v<A, color_t>)
            return get_color_restriction();
    }

    auto& get_color_restriction() const
    {
        return restrictions->colorRestriction;
    }
    auto& get_value_restriction() const
    {
        return restrictions->valueRestriction;
    }

    auto get_die() const { return die; }
    void put_die(Die* d) { die = d; }

    Die* remove_die()
    {
        Die* tmpDie = die;
        die = nullptr;
        return tmpDie;
    }
};

#endif
