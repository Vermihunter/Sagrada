#ifndef GAME_CACHE_INITIALIZER_H
#define GAME_CACHE_INITIALIZER_H

#include "BoardTextFileReader.h"
#include "GameCache.h"
#include "GameConfigReader.h"

#include <fstream>
#include <iostream>

/**
 * @brief Static initializer of game cache
 * Uses a static instance initialization that reads all
 * configs before any game object could be created
 */
struct GameCacheInitializer {
	static void initialize_cache();
};

#endif // GAME_CACHE_INITIALIZER_H