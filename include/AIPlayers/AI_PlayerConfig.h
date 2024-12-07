#ifndef AI_PLAYER_CONFIG_H
#define AI_PLAYER_CONFIG_H

#include "AI_PlayerContext.h"
#include "Game.h"
#include "PrivateObjectiveCard.h"
#include "Typedefs.h"

class AI_Player;

/**
 * @brief Base class interface for AI player configurations
 *
 * Once created and multiple instances of an AI player can be created.
 */
struct AI_PlayerConfig
{
    virtual ~AI_PlayerConfig() = default;

    /**
     * @brief Creates an AI player object
     *
     * @param _ctx Context containing the game specific informations
     * @return ai_player_t
     */
    virtual ai_player_t create_player(const AIPlayerContext& _ctx) const = 0;

    /**
     * @brief Clones the player config object
     *
     * @return ai_player_config_t
     */
    virtual ai_player_config_t clone() const = 0;

    /**
     * @defgroup BasicInformationQueries
     * This group contains functions that are used to query the type of the AI
     * player in different forms for different use cases
     * @{
     */
    virtual const std::string& get_name() const = 0;
    virtual const std::string& get_base_name() const = 0;
    virtual const std::string& get_monogram() const = 0;
    /**
     * @}
     */
};

#endif // AI_PLAYER_CONFIG_H