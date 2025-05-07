#ifndef PLAYER_STATISTICS_H
#define PLAYER_STATISTICS_H

#include "WilsonScoreInterval.h"

#include <algorithm>
#include <unordered_map>

struct TournamentStatistics;

/**
 * @brief A structure holding together the information that is remembered about
 * a player in a tournament
 */
struct PlayerStatistics {
	PlayerStatistics(TournamentStatistics& _statistics);
	~PlayerStatistics();

	WilsonScoreInterval get_wilson_score_interval_with_98_confidence() const;

	TournamentStatistics& statistics;
	unsigned int          winCount;
	unsigned int          totalPlacementPoints;
	int                   totalReceivedPoints;
	size_t                totalMoveTimeInMs;
	size_t                totalMoveCount;
};

#endif // PLAYER_STATISTICS_H