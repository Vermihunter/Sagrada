#ifndef SCORE_LOG_H
#define SCORE_LOG_H

#include "EvalState.h"
#include "Logger.h"

inline void LOG_SCORE(const EvalState& score)
{
	LOG_D("---------------------------------");
	LOG_D("Evaluating for the player with ID " + std::to_string(score.get_player()->ID));
	LOG_D("Unused favor token points: " + std::to_string(score.get_unused_favor_token_points()));
	LOG_D("Empty field minus points: " + std::to_string(score.get_empty_fields_points()));
	LOG_D("Private objective card points: " + std::to_string(score.get_proc_points()));

	size_t i = 0;
	for (auto&& puocState : score.get_puoc_state()) {
		LOG_D("PuOC with ind " + std::to_string(i) + " and ID " +
		      std::to_string(int(score.get_puoc()[i]->get_type())) + ": " +
		      std::to_string(puocState.totalEarnedPoints));
		++i;
	}

	LOG_D("Total points: " + std::to_string(score.get_total_points()));
	LOG_D("---------------------------------");
}

#endif // SCORE_LOG_H