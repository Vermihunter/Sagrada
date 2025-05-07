#ifndef GAME_CONTEXT_EQUAL_TEST_H
#define GAME_CONTEXT_EQUAL_TEST_H

#include "GameContext.h"
#include "PlayersEqualTest.h"
#include "RoundTrackEqualTest.h"
#include "SagradaTest.h"
#include "ScoreContextEqualTest.h"
#include "SupplyEqualTest.h"

void check_ctx_equal(const GameContext& ctx1, const GameContext& ctx2)
{
	EXPECT_EQ(ctx1.numberOfRounds, ctx2.numberOfRounds);
	EXPECT_EQ(ctx1.dicePerRound, ctx2.dicePerRound);
	EXPECT_EQ(ctx1.playerWpcChoiceCount, ctx2.playerWpcChoiceCount);

	check_players_equal(ctx1.players, ctx2.players);
	check_supply_equals(*ctx1.supply, *ctx2.supply);
	check_score_ctx_equals(*ctx1.scoreCtx, *ctx2.scoreCtx);
	check_round_track_equals(ctx1.roundTrack, ctx2.roundTrack);

	EXPECT_EQ(ctx1.toolCards.size(), ctx2.toolCards.size());
	for (size_t i = 0; i < ctx1.toolCards.size(); ++i) {
		EXPECT_EQ(typeid(*ctx1.toolCards[i]), typeid(*ctx2.toolCards[i]));
	}

	EXPECT_EQ(ctx1.publicObjectiveCards.size(), ctx2.publicObjectiveCards.size());
	for (size_t i = 0; i < ctx1.toolCards.size(); ++i) {
		EXPECT_EQ(typeid(*ctx1.publicObjectiveCards[i]), typeid(*ctx2.publicObjectiveCards[i]));
	}

	EXPECT_EQ(ctx1.selectableWPC.size(), ctx2.selectableWPC.size());
	// todo others
}

#endif // GAME_CONTEXT_EQUAL_TEST_H