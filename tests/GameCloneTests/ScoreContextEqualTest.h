#ifndef SCORE_CONTEXT_EQUAL_TEST_H
#define SCORE_CONTEXT_EQUAL_TEST_H

#include "SagradaTest.h"
#include "ScoreContext.h"

void check_score_ctx_equals(const ScoreContext& sctx1, const ScoreContext& sctx2)
{
	EXPECT_EQ(sctx1.minusPointsPerEmptyField, sctx2.minusPointsPerEmptyField);
	EXPECT_EQ(sctx1.pointsPerProcValue, sctx2.pointsPerProcValue);
	EXPECT_EQ(sctx1.pointsPerUnusedFavorToken, sctx2.pointsPerUnusedFavorToken);
}

#endif // SCORE_CONTEXT_EQUAL_TEST_H