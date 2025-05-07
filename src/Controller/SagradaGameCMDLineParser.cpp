#include "SagradaGameCMDLineParser.h"

#include "GameCache.h"

game_config_t SagradaGameCMDLineParser::get_game_config()
{
	game_config_t clonedGameConfig = std::make_unique<GameConfig>();

	for (auto&& player : gameConfig.players) {
		clonedGameConfig->players.push_back(player->clone());
	}

	size_t playerCount = gameConfig.players.size();
	auto   ctxName = get<std::string>("-mode"); // (parent ? parent->get<std::string>("-mode") : );
	clonedGameConfig->gameCtxBuilder = GameCache::get().get_context(playerCount, ctxName);
	if (!clonedGameConfig->gameCtxBuilder) {
		throw std::runtime_error("There is no context defined for " + std::to_string(playerCount) +
		                         " players and name " + ctxName);
	}

	clonedGameConfig->randomSeed        = get<int>("-s");
	clonedGameConfig->gameDeterministic = get<bool>("-d");

	return clonedGameConfig;
}