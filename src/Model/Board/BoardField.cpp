#include "BoardField.h"

BoardField::BoardField()
    : BoardField(std::make_unique<BoardFieldRestrictions>())
{
}

BoardField::BoardField(BoardField&& other)
    : restrictions(other.restrictions->clone()), die(other.die)
{
}

BoardField::BoardField(restriction_t _restrictions)
    : restrictions(std::move(_restrictions)), die(nullptr)
{
}

BoardField::BoardField(const BoardField& bf, DiceSupply& newDS,
                       DiceSupply& originalDS)
    : restrictions(bf.restrictions->clone())
{
    die = (bf.die) ? get_corresponding_die(bf.die, originalDS, newDS) : nullptr;
}
