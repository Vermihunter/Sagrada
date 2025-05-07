#ifndef ONLY_AI_PLAYER_CONTROLLER_H
#define ONLY_AI_PLAYER_CONTROLLER_H

#include "AI_Player.h"
#include "AI_PlayerNames.h"
#include "ControllerWithAIPlayer.h"
#include "GameContext.h"
#include "RandomGenerator.h"

#include <memory>

class OnlyAIPlayerController;
using only_ai_controller_t = std::unique_ptr<OnlyAIPlayerController>;

/**
 * @brief Controller that is used when there are only AI players in the game
 *
 */
class OnlyAIPlayerController : public ControllerWithAIPlayer {
  public:
	OnlyAIPlayerController(Game& _game, const ai_player_config_pc& configs,
	                       const bool gameDeterministic = false);

	/**
	 * @brief Runs the whole game and returns the controller
	 * The controller contains every information about the AI players needed to
	 * analyze the results
	 *
	 * @param _game The game state representing the context
	 * @param configs AI Player configs that will play in the game
	 * @return only_ai_controller_t The resulting controller
	 */
	only_ai_controller_t run_whole_game(Game& _game, const ai_player_config_pc& configs);

	/**
	 * @brief Get the player to config mapper that enables to reverse-engineer
	 * the corresponding context
	 *
	 * @return auto&
	 */
	auto& get_player_to_config_mapper() { return playerToConfig; }

  private:
	std::unordered_map<AI_Player*, AI_PlayerConfig*> playerToConfig;
};

#endif // ONLY_AI_PLAYER_CONTROLLER_H