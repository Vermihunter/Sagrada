#ifndef TOOL_CARD_TEST_H
#define TOOL_CARD_TEST_H

#include "Typedefs.h"
#include "GameContextBuilder.h"
#include "DefaultGameCreator.h"
#include "SagradaTest.h"


class ToolCardTest {

public:
    static game_t create_game_with_tc(tc_etype tc) {
        auto gameCtxBuilder = GameContextBuilder::create(
            DefaultGameCreator::get_default_player_count_related_config(), 
            DefaultGameCreator::get_default_puoc_context(),
            DefaultGameCreator::get_default_tc_context(),
            BoardConfig::get_config(5, 4, BoardType::get_type("Rectangle")));
        
        gameCtxBuilder->add_tc(tc);
        auto gameCtx = gameCtxBuilder->build(2, RandomGenerator::initialize_with_curr_time());
        return std::make_unique<Game>(std::move(gameCtx), DefaultGameCreator::get_default_game_state());
    }

private:


};

#endif // TOOL_CARD_TEST_H