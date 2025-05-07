#ifndef HEURISTIC_CONSTANTS_H
#define HEURISTIC_CONSTANTS_H

#include "SagradaJson.h"

/**
 * @brief Heuristic constants for the AI players that use heuristics
 *
 * Currently, these AI players are the Minimax player and the Rules-based
 * player.
 *
 * It can be configured to use a specific set of constants or to use the default
 * ones that are the results of an experiment.
 */
struct HeuristicConstants {
	double opponentHeuristicInfluencingFactor;
	double puocCompletablePower;
	int    minusPointsPerUncompletableField;
	int    completedPointsWeight;
	int    minusPerUncompletablePuocPoints;
};

/*
constexpr HeuristicConstants defaultConstants {
    .opponentHeuristicInfluencingFactor = 4.0,
    .puocCompletablePower = 2.0,
    .minusPointsPerUncompletableField = 150,
    .completedPointsWeight = 20,
    .minusPerUncompletablePuocPoints = 5
};
*/

/**
 * @brief The currently used default heuristic constants
 *
 */
constexpr HeuristicConstants defaultConstants{.opponentHeuristicInfluencingFactor =
                                                  9.83724229656198,
                                              .puocCompletablePower = 3.1133889400588606,
                                              .minusPointsPerUncompletableField = 373,
                                              .completedPointsWeight            = 416,
                                              .minusPerUncompletablePuocPoints  = 7};

constexpr int MINIMAX_defaultWorldCount = 4;

inline void from_json(const json& j, HeuristicConstants& constants)
{
	constants.opponentHeuristicInfluencingFactor = j.at("OpponentInfluencingFactor").get<double>();
	constants.puocCompletablePower               = j.at("PuocCompletablePower").get<double>();
	constants.minusPointsPerUncompletableField =
	    j.at("MinusPointsPerUncompletableField").get<int>();
	constants.completedPointsWeight = j.at("CompletedPointsWeight").get<int>();
	constants.minusPerUncompletablePuocPoints =
	    j.at("MinusPointsPerUncompletablePuocPoints").get<int>();
}

#endif // HEURISTIC_CONSTANTS_H