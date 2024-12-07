#ifndef ROUND_TRACK_EQUAL_TEST_H
#define ROUND_TRACK_EQUAL_TEST_H

#include "SagradaTest.h"
#include "DieEqualTest.h"
#include "RoundTrack.h"

void check_round_track_equals(const RoundTrack& rt1, const RoundTrack& rt2) {
    check_all_dice_equal(rt1.get_dice(), rt2.get_dice(), true);
}

#endif // ROUND_TRACK_EQUAL_TEST_H