#ifndef CHANGE_DIE_VALUE_TC_TEST_H
#define CHANGE_DIE_VALUE_TC_TEST_H

#include "ChangeDieValueTC.h"
#include "DieValueTestConfig.h"
#include "ToolCardTest.h"

class ChangeDieValueCorrectnessTest : public ::testing::TestWithParam<DieValueTestConfig> {};

TEST_P(ChangeDieValueCorrectnessTest, CorrectValueChange) {}

#endif // CHANGE_DIE_VALUE_TC_TEST_H