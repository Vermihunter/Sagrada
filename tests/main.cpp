#include "CloneTests.h"
#include "GameCacheInitializer.h"
#include "MoveTests.h"
#include "ReverseDieValueTCTest.h"
#include "UndoTest.h"

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    GameCacheInitializer::initialize_cache();
    return RUN_ALL_TESTS();
}
