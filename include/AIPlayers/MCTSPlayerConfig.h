#ifndef MCTS_PLAYER_CONFIG_H
#define MCTS_PLAYER_CONFIG_H

#include "AI_PlayerConfig.h"
#include "MCTSPlayerContext.h"

/**
 * @brief Player configuration for the MCTS player
 *
 */
struct MCTSPlayerConfig : public AI_PlayerConfig
{
  public:
    inline const static std::string type = "MCTS";
    inline const static std::string monogram = "MC";

    MCTSPlayerConfig(mcts_context_t _ctx);
    MCTSPlayerConfig(const MCTSPlayerConfig& other);

    virtual ai_player_t create_player(
        const AIPlayerContext& _ctx) const override;

    virtual const std::string& get_name() const override { return name; }
    virtual const std::string& get_base_name() const override { return type; }
    virtual const std::string& get_monogram() const override
    {
        return monogram;
    };

    virtual ai_player_config_t clone() const override
    {
        return std::make_unique<MCTSPlayerConfig>(*this);
    }

  private:
    mcts_context_t ctx;
    std::string name;
};

#endif // MCTS_PLAYER_CONFIG_H