#ifndef GAME_INFO_H
#define GAME_INFO_H

#include "Typedefs.h"

class Game;

/**
 * @brief Helper class for common operations
 */
struct GameInfo
{
    static const player_t& player_on_move(const Game& g);
    static const player_t& get_player_with_id(const Game& g, ID_t playerId);

    /**
     * @brief Get the player with id from players object
     * Strongly relies on the fact that player[i] has ID equal to i
     */
    static const player_t& get_player_with_id_from_players(
        const player_c& players, ID_t playerId);
};

#endif // GAME_INFO_H