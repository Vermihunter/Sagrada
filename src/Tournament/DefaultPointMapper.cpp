#include "DefaultPointMapper.h"

#include "TournamentConfig.h"

void DefaultPointMapper::initialize_for_config(TournamentConfig& config)
{
	size_t configPlayerCount = config.gameConfig->players.size();
	for (size_t i = 0; i < configPlayerCount; ++i) {
		config.pointsMapper[i] = configPlayerCount - i;
	}
}