#ifndef GAME_CACHE_H
#define GAME_CACHE_H

#include <unordered_map>

#include "BoardConfig.h"
#include "Typedefs.h"

/**
 * @brief Stores information shared between all games
 *
 * Follows the Meyersen singleton pattern
 * The information stored in the singleton object is read from
 * config files and define the usable configurations for the games.
 */
struct GameCache
{
    static GameCache& get()
    {
        static GameCache cache;
        return cache;
    }

    /**
     * @brief Get the wpc container
     * Not defined const because the concrete ones
     * are inserted using this function call
     */
    auto& get_wpc() { return wpcByBoardConfig; }

    /**
     * @brief Returns all the defined game contexts
     */
    auto& get_defined_game_contexts() { return definedContexts; }

    /**
     * @brief Helper function for functionality used in multiple places
     *
     * @param playerCount Number of players that context is defined for
     * @param ctxName Name of the context
     * @return cached_game_ctx_builder_t The context for given player count
     * having name ctxName
     */
    cached_game_ctx_builder_t get_context(size_t playerCount,
                                          const std::string& ctxName);

  private:
    wpc_cache_t wpcByBoardConfig;
    game_ctx_builder_by_player_count_t definedContexts;

    GameCache() {}
    GameCache(const GameCache&) = delete;
};

#endif // GAME_CACHE_H