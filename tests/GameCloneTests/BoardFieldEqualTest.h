#ifndef BOARD_FIELD_EQUAL_TEST_H
#define BOARD_FIELD_EQUAL_TEST_H

#include "BoardField.h"
#include "DieEqualTest.h"
#include "SagradaTest.h"

void check_board_fields_equal(const BoardField& field1, const BoardField& field2) {
    EXPECT_EQ(field1.get_value_restriction(), field2.get_value_restriction());
    EXPECT_EQ(field1.get_color_restriction(), field2.get_color_restriction());
    
    check_dice_equal_ptr(field1.get_die(), field2.get_die());
}

#endif // BOARD_FIELD_EQUAL_TEST_H