#ifndef DEFINED_AI_PLAYERS_H
#define DEFINED_AI_PLAYERS_H

#include "Typedefs.h"

#include <algorithm>
#include <memory>
#include <vector>

/**
 * @brief Represents a global representation of one configuration for each
 * defined player
 *
 * Singleton
 */
struct DefinedAIPlayers {
	static const ai_player_config_c& get()
	{
		static DefinedAIPlayers instance;
		return instance.definedPlayerConfigs;
	}

	static ai_player_config_t construct_by_name(const std::string& aiName);

  private:
	ai_player_config_c definedPlayerConfigs;

	DefinedAIPlayers();
};

#endif // DEFINED_AI_PLAYERS_H