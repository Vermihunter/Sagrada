#ifndef MINIMAX_PLAYER_CONFIG_H
#define MINIMAX_PLAYER_CONFIG_H

#include "AI_PlayerConfig.h"
#include "HeuristicConstants.h"

/**
 * @brief Player configuration for the Minimax player
 *
 */
struct MinimaxPlayerConfig : public AI_PlayerConfig
{
  public:
    inline const static std::string type = "Minimax";
    inline const static std::string monogram = "MI";

    static constexpr size_t defaultDepth = 5;

    MinimaxPlayerConfig(size_t _depth, const HeuristicConstants& _constants,
                        int _worldCount);

    virtual ai_player_t create_player(
        const AIPlayerContext& _ctx) const override;

    virtual const std::string& get_name() const override
    {
        return nameWithDepth;
    }
    virtual const std::string& get_base_name() const override { return type; }
    virtual const std::string& get_monogram() const override
    {
        return monogram;
    };

    virtual ai_player_config_t clone() const override
    {
        return std::make_unique<MinimaxPlayerConfig>(*this);
    }

  private:
    size_t depth;
    HeuristicConstants constants;
    int worldCount;
    std::string nameWithDepth;
};

#endif