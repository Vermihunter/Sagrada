#ifndef BOARD_CONTEXT_H
#define BOARD_CONTEXT_H

#include <memory>
#include <unordered_map>

#include "BoardConfig.h"
#include "BoardField.h"
#include "BoardIndex.h"
#include "Typedefs.h"

struct BoardContext;
using board_ctx_t = std::unique_ptr<BoardContext>;
struct BoardContext
{
    BoardContext() = default;
    BoardContext(const BoardContext& other);
    BoardContext(BoardContext& other, DiceSupply& newDiceSupply,
                 DiceSupply& originalDiceSupply);

    ~BoardContext();

    /**
     * @brief Clone the base of the context
     *
     * @return board_ctx_t
     */
    board_ctx_t clone_without_dice() const;

    /**
     * @brief Clone from one game to another
     *
     * @param newDiceSupply The dice supply of the new game
     * @param originalDiceSupply The dice supply of the original game
     * @return board_ctx_t
     */
    board_ctx_t clone(DiceSupply& newDiceSupply,
                      DiceSupply& originalDiceSupply);

    board_config_t config;
    board_data_t data;
    unsigned int hardness;
    std::string title;
};

#endif // BOARD_CONTEXT_H