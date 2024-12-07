#ifndef CHANGE_DIE_VALUE_TC_TEST_H
#define CHANGE_DIE_VALUE_TC_TEST_H

#include "ToolCardTest.h"
#include "ChangeDieValueTC.h"
#include "DieValueTestConfig.h"


class ChangeDieValueCorrectnessTest : public ::testing::TestWithParam<DieValueTestConfig>{};


TEST_P(ChangeDieValueCorrectnessTest, CorrectValueChange) {

}

#endif // CHANGE_DIE_VALUE_TC_TEST_H