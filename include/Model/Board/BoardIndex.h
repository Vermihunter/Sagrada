#ifndef BOARD_INDEX_H
#define BOARD_INDEX_H

#include <functional>
#include <iostream>
#include <vector>

#include "Constants.h"

struct BoardIndex
{
    using index_t = size_t;

    index_t row;
    index_t col;

    BoardIndex(index_t _row, index_t _col) : row(_row), col(_col) {}
    BoardIndex() : BoardIndex(0, 0) {}

    /** Structure binding interface begin */
    static constexpr std::size_t tuple_size = 2;

    template <std::size_t I>
    using tuple_element = index_t&;

    template <size_t index>
    constexpr index_t& get()
    {
        if constexpr (index == 0)
            return row;
        if constexpr (index == 1)
            return col;
    }

    /** Structure binding interface end */

    /** Debug */
    friend std::ostream& operator<<(std::ostream& os, const BoardIndex& ind)
    {
        os << "[" << ind.row << "," << ind.col << "]";
        return os;
    }

    friend bool operator==(const BoardIndex& lhs, const BoardIndex& rhs)
    {
        return lhs.row == rhs.row && lhs.col == rhs.col;
    }

    friend bool operator<(const BoardIndex& lhs, const BoardIndex& rhs)
    {
        if (lhs.row != rhs.row) {
            return lhs.row < rhs.row;
        }

        return lhs.col < rhs.col;
    }
};

/** std::unordered_map key interface */
template <>
struct std::hash<BoardIndex>
{
    std::size_t operator()(const BoardIndex& ind) const
    {
        std::size_t h1 = std::hash<BoardIndex::index_t> {}(ind.row);
        std::size_t h2 = std::hash<BoardIndex::index_t> {}(ind.col);

        return h1 ^ h2;
    }
};

using board_ind_c = std::vector<BoardIndex>;

#endif