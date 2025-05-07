#ifndef PLAYERS_EQUAL_TEST_H
#define PLAYERS_EQUAL_TEST_H

#include "BoardsEqualTest.h"
#include "SagradaTest.h"

void check_player_equal(const player_t& player1, const player_t& player2)
{
	EXPECT_EQ(player1->ID, player2->ID);
	EXPECT_EQ(player1->name, player2->name);
	EXPECT_EQ(player1->PrOC->get_color(), player2->PrOC->get_color());
	EXPECT_EQ(player1->favorTokensLeft, player2->favorTokensLeft);

	check_boards_equal(*player1->board, *player2->board);
}

void check_players_equal(const player_c& players1, const player_c& players2)
{
	EXPECT_EQ(players1.size(), players2.size());
	for (size_t i = 0; i < players1.size(); ++i) {
		check_player_equal(players1[i], players2[i]);
	}
}

#endif // PLAYERS_EQUAL_TEST_H