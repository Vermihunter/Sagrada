#include "Board.h"

Board::Board(const Board& b, DiceSupply& newDS, DiceSupply& originalDS)
{
    _ctx = b._ctx->clone(newDS, originalDS);
    fastctx = _ctx.get();
}

/**
 * @brief
 */
unsigned int Board::get_total_dice_value_having_color(color_t c)
{
    unsigned int counter = 0;

    for (auto&& row : fastctx->data) {
        for (auto&& field : row) {
            auto die = field->get_die();
            if (die && die->get_color() == c) {
                /** The enum value of a value_t is one less than the logic value
                 * (ONE has value of 0, ...) */
                counter += (size_t(die->get_value()) + 1);
            }
        }
    }

    return counter;
}

/**
 * @brief Calculates the number of empty fields in a board
 */
unsigned int Board::get_number_of_empty_fields()
{
    unsigned int count = 0;
    for (auto&& row : fastctx->data) {
        for (auto&& field : row) {
            if (!field->get_die()) {
                ++count;
            }
        }
    }

    return count;
}

std::vector<BoardIndex> Board::get_empty_fields()
{
    std::vector<BoardIndex> res;

    BoardIndex ind {0, 0};
    for (auto&& row : fastctx->data) {
        for (auto&& field : row) {
            if (!field->get_die()) {
                res.push_back(ind);
            }
            ++ind.col;
        }
        ind.col = 0;
        ++ind.row;
    }

    return res;
}

bool Board::is_empty()
{
    for (auto&& row : fastctx->data) {
        for (auto&& field : row) {
            if (field->get_die()) {
                return false;
            }
        }
    }

    return true;
}