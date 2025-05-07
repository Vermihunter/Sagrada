#include "OnlyAIPlayerController.h"

OnlyAIPlayerController::OnlyAIPlayerController(Game& _game, const ai_player_config_pc& configs,
                                               const bool gameDeterministic)
    : ControllerWithAIPlayer(_game)
{
	ID_t playerInd = 0;
	for (auto&& player : game.get_ctx()->players) {
		player->name =
		    "Player" + std::to_string(playerInd) + "(" + configs[playerInd]->get_monogram() + ")";
		auto ai_player =
		    AI_Player::initialize_player(game, *configs[playerInd], player, gameDeterministic);
		playerToConfig[ai_player.get()] = configs[playerInd];
		AI_PlayersByID[player->ID]      = std::move(ai_player);
		++playerInd;
	}
}

only_ai_controller_t OnlyAIPlayerController::run_whole_game(Game&                      _game,
                                                            const ai_player_config_pc& configs)
{
	auto controller = std::make_unique<OnlyAIPlayerController>(_game, configs);

	while (!controller->game.get_curr_state()->hasEnded) {
		controller->make_next_ai_player_move();
	}

	return controller;
}