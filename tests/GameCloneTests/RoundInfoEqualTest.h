#ifndef ROUND_INFO_EQUAL_TEST_H
#define ROUND_INFO_EQUAL_TEST_H

#include "GameHistory.h"
#include "SagradaTest.h"

void check_turn_info_equals(const TurnInfo& ri1, const TurnInfo& ri2) {
    EXPECT_EQ(ri1.playerId, ri2.playerId);
    EXPECT_EQ(ri1.tcUsed, ri2.tcUsed);
    EXPECT_EQ(ri1.dieOriginalInd, ri2.dieOriginalInd);
}

void check_round_info_equals(const RoundInfo& ri1, const RoundInfo& ri2) {
    EXPECT_EQ(ri1.turnInfos.size(), ri2.turnInfos.size());

    for(size_t i = 0; i < ri1.turnInfos.size(); ++i) {
        check_turn_info_equals(ri1.turnInfos[i], ri2.turnInfos[i]);
    }
}

#endif // ROUND_INFO_EQUAL_TEST_H