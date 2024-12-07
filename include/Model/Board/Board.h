#ifndef BOARD_H
#define BOARD_H

#include <array>

#include "Attributes.h"
#include "BoardContext.h"
#include "BoardIndex.h"
#include "Constants.h"
#include "Logger.h"
#include "Typedefs.h"

/**
 * @brief A board in an ongoing game representing the current
 * state for a given player.
 */
class Board
{
  public:
    Board(board_ctx_t __ctx)
    {
        _ctx = std::move(__ctx);
        fastctx = _ctx.get();
    }

    auto from_ctx(board_ctx_t ctx)
    {
        return std::make_unique<Board>(std::move(ctx));
    }

    Board(const Board& b, DiceSupply& newDS, DiceSupply& originalDS);

    auto clone(DiceSupply& newDS, DiceSupply& originalDS)
    {
        return std::make_unique<Board>(*this, newDS, originalDS);
    }

    /**
     * @brief
     *
     * @param index
     * @return board_field_t&
     */
    auto& operator[](const BoardIndex& index) const
    {
        return fastctx->data[index.row][index.col];
    }

    /**
     * @brief Counts the fields that don't have a die on
     *
     * It is a helper function called during evaluation process
     *
     * @return The count of empty fields
     */
    unsigned int get_number_of_empty_fields();

    /**
     * @brief Returns all the indices of the fields not containing a die
     */
    std::vector<BoardIndex> get_empty_fields();

    /**
     * @brief Makes the API nicer
     */
    bool is_empty();

    /**
     * @brief Counts the total value of dice which have a given color
     *
     * It is a helper function called during evaluation process
     *
     * @param c The color of the dice to calculate
     * @return The total count of dice values having color c
     */
    unsigned int get_total_dice_value_having_color(color_t c);

    const auto& get_ctx() const { return fastctx; }

  private:
    board_ctx_t _ctx;
    BoardContext* fastctx;
};

#endif // BOARD_H