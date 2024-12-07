#ifndef RANDOM_PLAYER_CONFIG_H
#define RANDOM_PLAYER_CONFIG_H

#include "AI_PlayerConfig.h"
#include "RandomPlayer.h"

/**
 * @brief Player configuration for the Random player
 *
 */
struct RandomPlayerConfig : public AI_PlayerConfig
{
  public:
    inline const static std::string type = "Random";
    inline const static std::string monogram = "RN";

    virtual ai_player_t create_player(
        const AIPlayerContext& _ctx) const override
    {
        return std::make_unique<RandomPlayer>(_ctx);
    }

    virtual const std::string& get_name() const override { return type; }
    virtual const std::string& get_base_name() const override { return type; }
    virtual const std::string& get_monogram() const override
    {
        return monogram;
    };

    virtual ai_player_config_t clone() const override
    {
        return std::make_unique<RandomPlayerConfig>(*this);
    }
};

#endif // RANDOM_PLAYER_CONFIG_H