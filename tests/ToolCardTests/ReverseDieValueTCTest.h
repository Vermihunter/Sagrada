#ifndef REVERSE_DIE_VALUE_TC_TEST_H
#define REVERSE_DIE_VALUE_TC_TEST_H

#include "ToolCardTest.h"
#include "ReverseDieValueTC.h"
#include "DieValueTestConfig.h"
#include "StringAttributes.h"


class ReverseDieValueReversingValueTest : public ::testing::TestWithParam<DieValueTestConfig> {};

TEST_P(ReverseDieValueReversingValueTest, CorrectValueChange) {
    DieValueTestConfig config = GetParam();
    Die hypotheticalDie {color_t::BLUE, config.originalValue};
    ReverseDieValueTC::reverse_die_value(&hypotheticalDie);

    ASSERT_EQ(config.expectedValue, hypotheticalDie.get_value());
}

INSTANTIATE_TEST_SUITE_P(ValueReversingTests, ReverseDieValueReversingValueTest,
    ::testing::Values(
        DieValueTestConfig{.originalValue = value_t::ONE, .expectedValue = value_t::SIX},
        DieValueTestConfig{.originalValue = value_t::TWO, .expectedValue = value_t::FIVE},
        DieValueTestConfig{.originalValue = value_t::THREE, .expectedValue = value_t::FOUR},
        DieValueTestConfig{.originalValue = value_t::FOUR, .expectedValue = value_t::THREE},
        DieValueTestConfig{.originalValue = value_t::FIVE, .expectedValue = value_t::TWO},
        DieValueTestConfig{.originalValue = value_t::SIX, .expectedValue = value_t::ONE}
    ),
    [](const testing::TestParamInfo<DieValueTestConfig>& info) {
        return "Test_" + value_to_string.at(info.param.originalValue);
    });

#endif // REVERSE_DIE_VALUE_TC_TEST_H