#ifndef BOARDS_EQUAL_TEST_H
#define BOARDS_EQUAL_TEST_H

#include "Board.h"
#include "BoardFieldEqualTest.h"
#include "SagradaTest.h"

void check_boards_equal(const Board& b1, const Board& b2) {
    const auto& ctx1 = *b1.get_ctx();
    const auto& ctx2 = *b2.get_ctx();

    // Board config tests
    EXPECT_EQ(ctx1.config->width, ctx2.config->width);
    EXPECT_EQ(ctx1.config->height, ctx2.config->height);
    EXPECT_EQ(ctx1.config->type, ctx2.config->type);

    // Other data tests
    EXPECT_EQ(ctx1.title, ctx2.title);
    EXPECT_EQ(ctx1.hardness, ctx2.hardness);

    // Data fields check
    EXPECT_EQ(ctx1.data.size(), ctx2.data.size());
    for(size_t i = 0; i < ctx1.data.size(); ++i) {
        EXPECT_EQ(ctx1.data[i].size(), ctx2.data[i].size());

        for(size_t j = 0; j < ctx1.data[i].size(); ++j) {
            check_board_fields_equal(*ctx1.data[i][j], *ctx2.data[i][j]);
        }
    }

}


#endif // BOARDS_EQUAL_TEST_H