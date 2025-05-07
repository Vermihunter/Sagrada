#include "GameCacheInitializer.h"

void GameCacheInitializer::initialize_cache()
{
	std::ifstream f{};
	f.open(WPCConfigPath);

	BoardTextFileReader reader{f};

	auto& wpcCache = GameCache::get().get_wpc();
	reader.append_wpc(wpcCache);

	auto& definedCtxs = GameCache::get().get_defined_game_contexts();
	definedCtxs       = GameConfigReader::get_all_available_game_contexts();
}