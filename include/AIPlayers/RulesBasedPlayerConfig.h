#ifndef RULES_BASED_PLAYER_CONFIG_H
#define RULES_BASED_PLAYER_CONFIG_H

#include "AI_Player.h"
#include "RulesBasedPlayer.h"

/**
 * @brief Player configuration for the Rules-based player
 *
 */
class RulesBasedPlayerConfig : public AI_PlayerConfig {
  public:
	inline const static std::string type     = "RulesBased";
	inline const static std::string monogram = "RB";

	RulesBasedPlayerConfig(RulesBasedPlayer::strategy_t _strategy);

	virtual ai_player_t create_player(const AIPlayerContext& _ctx) const override;

	virtual const std::string& get_name() const override { return name; }
	virtual const std::string& get_base_name() const override { return type; }
	virtual const std::string& get_monogram() const override { return monogram; };

	virtual ai_player_config_t clone() const override
	{
		return std::make_unique<RulesBasedPlayerConfig>(*this);
	}

  private:
	RulesBasedPlayer::strategy_t strategy;
	std::string                  name;
};

#endif // RULES_BASED_PLAYER_CONFIG_H