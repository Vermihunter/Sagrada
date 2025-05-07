#ifndef SUPPLY_EQUAL_TEST_H
#define SUPPLY_EQUAL_TEST_H

#include "DiceSupply.h"
#include "SagradaTest.h"

void check_supply_equals(const DiceSupply& supply1, const DiceSupply& supply2)
{
	const auto& dice1 = supply1.get_all_dice();
	const auto& dice2 = supply2.get_all_dice();

	EXPECT_EQ(dice1.size(), dice2.size());

	for (size_t i = 0; i < dice1.size(); ++i) {
		check_dice_equal(dice1[i], dice2[i]);
	}
}

#endif // SUPPLY_EQUAL_TEST_H