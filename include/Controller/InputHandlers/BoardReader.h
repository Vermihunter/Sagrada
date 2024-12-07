#ifndef BOARD_READER_H
#define BOARD_READER_H

#include "Attributes.h"
#include "BoardContext.h"
#include "BoardFieldRestrictions.h"
#include "Typedefs.h"

/**
 * @brief Interface for any type of board readers
 *
 */
struct BoardReader
{
    virtual ~BoardReader() = default;
    virtual void append_wpc(wpc_cache_t& dest) = 0;

  protected:
    /**
     * @brief Create a board config object
     * Memory leak, but it doesnt matter because it is lightweight and is
     * present for the whole run of the process.
     */
    template <typename... Params>
    board_config_t create_board_config(Params&&... params)
    {
        return new BoardConfig(std::forward<Params>(params)...);
    }
};

/**
 * @brief Basic mapping function
 *
 * @param c Input character
 * @return restriction_t Output restriction container
 */
inline restriction_t char_to_restriction(char c)
{
    auto restr = std::make_unique<BoardFieldRestrictions>();

    switch (c) {
        // Characters indicating color
        case 'Y':
            restr->colorRestriction = color_t::YELLOW;
            break;
        case 'G':
            restr->colorRestriction = color_t::GREEN;
            break;
        case 'B':
            restr->colorRestriction = color_t::BLUE;
            break;
        case 'P':
            restr->colorRestriction = color_t::PURPLE;
            break;
        case 'R':
            restr->colorRestriction = color_t::RED;
            break;

        // Characters indicating value
        case '1':
            restr->valueRestriction = value_t::ONE;
            break;
        case '2':
            restr->valueRestriction = value_t::TWO;
            break;
        case '3':
            restr->valueRestriction = value_t::THREE;
            break;
        case '4':
            restr->valueRestriction = value_t::FOUR;
            break;
        case '5':
            restr->valueRestriction = value_t::FIVE;
            break;
        case '6':
            restr->valueRestriction = value_t::SIX;
            break;

        // Character indicates field without restriction
        case '_':
            break;
        // Invalid character
        default:
            // Log something bad
            break;
    }

    return restr;
}

#endif // BOARD_READER_H