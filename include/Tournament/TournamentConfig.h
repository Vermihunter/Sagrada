#ifndef TOURNAMENT_CONFIG_H
#define TOURNAMENT_CONFIG_H

#include "DefaultPointMapper.h"
#include "GameConfig.h"
#include "ITournamentPrinter.h"
#include "Typedefs.h"

#include <iostream>
#include <memory>
#include <vector>

using placement_to_points_mapper_t = std::unordered_map<unsigned int, size_t>;

/**
 * @brief Contains data about the tournament
 */
struct TournamentConfig {
	TournamentConfig() { verboseLevel = 0; }

	int                                              verboseLevel;
	int                                              gameCount;
	bool                                             allDirections;
	bool                                             saveMoveStatistics;
	bool                                             saveGameInformation;
	game_config_t                                    gameConfig;
	placement_to_points_mapper_t                     pointsMapper;
	std::string                                      output;
	std::vector<std::unique_ptr<ITournamentPrinter>> tournamentPrinters;
	std::unique_ptr<std::ostream>                    logStream;
};

using tournament_config_t = std::unique_ptr<TournamentConfig>;

#endif // TOURNAMENT_CONFIG_H