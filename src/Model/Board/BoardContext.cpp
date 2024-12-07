#include "BoardContext.h"

BoardContext::BoardContext(BoardContext& other, DiceSupply& newDiceSupply,
                           DiceSupply& originalDiceSupply)
    : config(other.config), hardness(other.hardness), title(other.title)
{
    data.resize(other.data.size());
    int rowInd = 0;
    for (auto&& row : other.data) {
        for (auto&& field : row) {
            data[rowInd].push_back(
                field->clone(newDiceSupply, originalDiceSupply));
        }
        ++rowInd;
    }
}

BoardContext::BoardContext(const BoardContext& other)
    : config(other.config), hardness(other.hardness), title(other.title)
{
    data.resize(other.data.size());
    int rowInd = 0;
    for (auto&& row : other.data) {
        for (auto&& field : row) {
            data[rowInd].push_back(field->clone_without_die());
        }
        ++rowInd;
    }
}

BoardContext::~BoardContext()
{
    for (auto&& row : data) {
        for (auto&& field : row) {
            delete field;
        }
    }
}

board_ctx_t BoardContext::clone_without_dice() const
{
    return std::make_unique<BoardContext>(*this);
}

board_ctx_t BoardContext::clone(DiceSupply& newDiceSupply,
                                DiceSupply& originalDiceSupply)
{
    return std::make_unique<BoardContext>(*this, newDiceSupply,
                                          originalDiceSupply);
}