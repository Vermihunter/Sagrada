#ifndef FIRST_PLAYER_CONFIG_H
#define FIRST_PLAYER_CONFIG_H

#include "AI_PlayerConfig.h"
#include "FirstPlayer.h"

/**
 * @brief Config for the First AI player
 *
 */
struct FirstPlayerConfig : public AI_PlayerConfig {
	virtual ai_player_t create_player(const AIPlayerContext& _ctx) const override
	{
		return std::make_unique<FirstPlayer>(_ctx);
	}

	virtual const std::string& get_name() const override { return type; }
	virtual const std::string& get_base_name() const override { return type; }
	virtual const std::string& get_monogram() const override { return monogram; };

	virtual ai_player_config_t clone() const override
	{
		return std::make_unique<FirstPlayerConfig>(*this);
	}

  private:
	inline const static std::string type     = "First";
	inline const static std::string monogram = "RB";
};

#endif // FIRST_PLAYER_CONFIG_H