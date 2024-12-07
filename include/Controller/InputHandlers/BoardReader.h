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


#endif // BOARD_READER_H