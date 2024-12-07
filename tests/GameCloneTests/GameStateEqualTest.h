#ifndef GAME_STATE_EQUAL_TEST_H
#define GAME_STATE_EQUAL_TEST_H

#include "GameState.h"
#include "DieEqualTest.h"
#include "RoundInfoEqualTest.h"
#include "SagradaTest.h"

void check_states_equal(const GameState& state1, const GameState& state2) {
    EXPECT_EQ(state1.hasEnded, state2.hasEnded);
    EXPECT_EQ(state1.roundNumber, state2.roundNumber);
    EXPECT_EQ(state1.playerStartedThisRoundIndex, state2.playerStartedThisRoundIndex);
    EXPECT_EQ(state1.currPlayerIndex, state2.currPlayerIndex);
    EXPECT_EQ(state1.hasEnded, state2.hasEnded);

    check_all_dice_equal(state1.currentRoundRemainingDice, state2.currentRoundRemainingDice, true);
    check_round_info_equals(*state1.currRoundInfo, *state2.currRoundInfo);
}


#endif // GAME_STATE_EQUAL_TEST_H