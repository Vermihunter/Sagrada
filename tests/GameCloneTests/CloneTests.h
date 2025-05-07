#ifndef CLONE_TESTS_H
#define CLONE_TESTS_H

#include "DefaultGameCreator.h"
#include "GameContextEqualTest.h"
#include "GameStateEqualTest.h"

#include <gtest/gtest.h>
#include <typeinfo>

void check_games_equal(const Game& g1, const Game& g2)
{
	check_ctx_equal(*g1.get_ctx(), *g2.get_ctx());
	check_states_equal(*g1.get_curr_state(), *g2.get_curr_state());
	// history is ignored yet
}

class GameCloneSuite : public ::testing::Test {
  public:
	GameCloneSuite()
	{
		game = DefaultGameCreator::create(2, RandomGenerator::initialize_with_seed(55));

		for (auto&& player : game->get_ctx()->players) {
			auto [wpcItBegin, wpcItEnd] = game->get_player_window_pattern_card_options(player->ID);
			game->set_board(player->ID,
			                std::make_shared<Board>((*wpcItBegin)->clone_without_dice()));
		}
	}

	game_t game;
};

TEST_F(GameCloneSuite, TwoClonesOfSameGame)
{
	auto clone1 = game->clone();
	auto clone2 = game->clone();
	check_games_equal(*clone1, *clone2);
}

TEST_F(GameCloneSuite, CloneOfClone)
{
	auto clone1 = game->clone();
	auto clone2 = clone1->clone();
	check_games_equal(*clone1, *clone2);
}

TEST_F(GameCloneSuite, AfterMoveAndClonedMove)
{

	auto clonedGame = game->clone();

	auto move = game->possible_moves()[0];
	game->move_request(move);

	auto clonedMove = move->clone(*game->get_ctx()->supply, *clonedGame->get_ctx()->supply);
	clonedGame->move_request(clonedMove);

	check_games_equal(*game, *clonedGame);
}

TEST_F(GameCloneSuite, CloneAfterSingleMove)
{
	auto moves = game->possible_moves();
	game->move_request(moves[0]);

	auto clonedGame = game->clone();
	check_games_equal(*game, *clonedGame);
}

TEST_F(GameCloneSuite, FreshlyCreatedWithoutMoves)
{
	auto clonedGame = game->clone();
	check_games_equal(*game, *clonedGame);
}

#endif // CLONE_TESTS_H