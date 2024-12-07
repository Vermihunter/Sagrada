#ifndef MOVE_TESTS_H
#define MOVE_TESTS_H

#include "DefaultGameCreator.h"
#include "SagradaTest.h"

class MoveTest : public ::testing::TestWithParam<int> {

};
/*
TEST_P(MoveTest, AllGeneratedCorrect) {
  int seed = GetParam();
  game_t game = DefaultGameCreator::create(2, RandomGenerator::initialize_with_seed(seed));

  while(!game->get_curr_state()->hasEnded) {
    move_c possibleMoves = game->possible_moves();
    for(auto&& possibleMove : possibleMoves) {
        game_t gameClone = game->clone();
        move_t moveClone = possibleMove->clone(*game->get_ctx()->supply, *gameClone->get_ctx()->supply);
        EXPECT_NO_THROW({
            gameClone->move_request(moveClone);
        });
    }

    game->move_request(possibleMoves[possibleMoves.size() - 1]);
  }
  
  //EXPECT_TRUE(foo.Blah(GetParam()));
}
*/


#endif // MOVE_TESTS_H