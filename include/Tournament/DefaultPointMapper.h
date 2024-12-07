#ifndef DEFAULT_TOURNAMENT_POINTS_MAPPER_H
#define DEFAULT_TOURNAMENT_POINTS_MAPPER_H

#include <unordered_map>

struct TournamentConfig;

/**
 * @brief Represents the default point system of the tournaments
 *
 * Tournaments contain this additional information that maps placements in the
 * following way. The points are evenly distributed - the first place gets the
 * same amount of points as there are players in the tournament and the last
 * player gets 1 point.
 */
struct DefaultPointMapper
{
    static void initialize_for_config(TournamentConfig& tournamentConfig);
};

#endif // DEFAULT_TOURNAMENT_POINTS_MAPPER_H