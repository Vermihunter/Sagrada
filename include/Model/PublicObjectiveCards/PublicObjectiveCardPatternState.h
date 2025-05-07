#ifndef PUBLIC_OBJECTIVE_CARD_PATTERN_STATE_H
#define PUBLIC_OBJECTIVE_CARD_PATTERN_STATE_H

#include <iostream>
#include <vector>

struct PublicObjectiveCard;

/**
 * @brief The main class representing the state of a board from a given Puoc's
 * perspective
 *
 */
struct PuocPatternState {
	PuocPatternState(const PublicObjectiveCard& _puoc);
	~PuocPatternState();

	/**
	 * @brief Contains information about subpatterns that are completable
	 */
	struct CompletableSubPattern {
		int diceToComplete;
	};

	friend std::ostream& operator<<(std::ostream& os, const PuocPatternState& pattern)
	{
		os << "Total earned points: " << pattern.totalEarnedPoints
		   << " uncompletable pattern count: " << pattern.uncompletableSubPatternCount
		   << " completable patterns: [";

		for (auto&& completablePattern : pattern.completableSubPatterns) {
			os << "{diceToComplete = " << completablePattern.diceToComplete << "},";
		}

		return os << "]";
	}

	/* Corresponding Puoc*/
	const PublicObjectiveCard& puoc;
	/* Sub-patterns that are completable */
	std::vector<CompletableSubPattern> completableSubPatterns;
	/* Constant */
	unsigned int dicePerPattern;
	/* Currently already matched pattern count multiplied with the pattern
	 * satisfaction value */
	unsigned int totalEarnedPoints;
	/* The number of subpaterns that cannot be completed according to the
	 * current state of the board
	 */
	unsigned int uncompletableSubPatternCount;
};

using puoc_pattern_t = PuocPatternState;

#endif // PUBLIC_OBJECTIVE_CARD_PATTERN_STATE_H