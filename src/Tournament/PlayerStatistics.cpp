#include "PlayerStatistics.h"

#include "TournamentStatistics.h"

PlayerStatistics::PlayerStatistics(TournamentStatistics& _statistics)
    : statistics(_statistics), winCount(0), totalPlacementPoints(0), totalReceivedPoints(0),
      totalMoveTimeInMs(0), totalMoveCount(0)
{
}

WilsonScoreInterval PlayerStatistics::get_wilson_score_interval_with_98_confidence() const
{
	constexpr double z_score   = 2.3263478740408408;
	constexpr double z_score_2 = z_score * z_score;

	double n         = statistics.gameResults.size();
	double n_success = winCount;
	double n_failure = n - n_success;

	double firstPart = (n_success + (z_score_2 / 2)) / (n + z_score_2);
	double secondPart =
	    (z_score / (n + z_score_2)) * std::sqrt((n_success * n_failure) / n + (z_score_2 / 4));

	double p_m = firstPart - secondPart;
	double p_p = firstPart + secondPart;

	return WilsonScoreInterval{
	    .confidence = 98, .z_value = z_score, .interval_low = p_m, .interval_high = p_p};
}

PlayerStatistics::~PlayerStatistics() {}