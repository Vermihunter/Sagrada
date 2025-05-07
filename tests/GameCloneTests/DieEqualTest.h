#ifndef DIE_EQUAL_TEST_H
#define DIE_EQUAL_TEST_H

#include "Die.h"
#include "SagradaTest.h"

void check_dice_equal(const Die& die1, const Die& die2)
{
	EXPECT_EQ(die1.get_color(), die2.get_color());
	EXPECT_EQ(die1.get_value(), die2.get_value());
}

void check_dice_equal_ptr(const Die* die1, const Die* die2)
{
	if (die1 == nullptr) {
		EXPECT_EQ(die2, nullptr);
	}
	else {
		EXPECT_NE(die2, nullptr);
		check_dice_equal(*die1, *die2);
	}
}

void check_all_dice_equal(const std::vector<Die*>& dice1, const std::vector<Die*>& dice2,
                          bool mustntNullptr)
{
	EXPECT_EQ(dice1.size(), dice2.size());

	for (size_t i = 0; i < dice1.size(); ++i) {
		if (mustntNullptr) {
			EXPECT_NE(dice1[i], nullptr);
			EXPECT_NE(dice2[i], nullptr);
			check_dice_equal(*dice1[i], *dice2[i]);
		}
		else {
			check_dice_equal_ptr(dice1[i], dice2[i]);
		}
	}
}

#endif // DIE_EQUAL_TEST_H