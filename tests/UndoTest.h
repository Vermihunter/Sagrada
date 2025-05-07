#ifndef UNDO_TEST_H
#define UNDO_TEST_H

#include "GameStateEqualTest.h"
#include "SagradaTest.h"
#include "ToolCardTest.h"

inline void send_tc_move(Game& g)
{
	move_c possibleMoves = g.possible_moves();
	for (auto&& move : possibleMoves) {
		if (move->t != Move::type::TOOL_CARD) {
			continue;
		}

		g.move_request(move);
		break;
	}
}

TEST(UndoTest, TC_6Test)
{

	game_t game = ToolCardTest::create_game_with_tc(tc_etype::RANDOM_BAG_DIE);
	for (auto&& player : game->get_ctx()->players) {
		auto [wpcItBegin, wpcItEnd] = game->get_player_window_pattern_card_options(player->ID);
		game->set_board(player->ID, std::make_shared<Board>((*wpcItBegin)->clone_without_dice()));
	}

	/* dont use clone_identical()
	game_t identicalGame = game->clone_identical();

	send_tc_move(*game);
	send_tc_move(*game);
	game->undo_last_move();
	game->undo_last_move();

	check_games_equal(*game, *identicalGame);
	*/
}

#endif // UNDO_TEST_H