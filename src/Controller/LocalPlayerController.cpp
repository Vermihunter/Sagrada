#include "LocalPlayerController.h"

#include "AI_PlayerNames.h"

local_player_controller_t LocalPlayerController::with_players(size_t localPlayerCount,
                                                              const ai_player_config_c& configs,
                                                              Game&                     game,
                                                              const bool gameDeterministic)
{
	auto controller = std::unique_ptr<LocalPlayerController>(new LocalPlayerController(game));

	auto& gameCtx                     = game.get_ctx();
	auto  concreteAiPlayerNameIndices = controller->game.get_ctx()->rnd->generate_random_id_order(
	     gameCtx->players.size() - localPlayerCount);

	ID_t playerInd = 0;
	for (auto&& player : gameCtx->players) {
		if (playerInd < localPlayerCount) {
			controller->localPlayerIDs.push_back(player->ID);
			player->name = "You";
		}
		else {
			controller->AI_PlayersByID[player->ID] = AI_Player::initialize_player(
			    controller->get_game(), *configs[playerInd - localPlayerCount], player,
			    gameDeterministic);
			player->name =
			    AI_PlayerNames[concreteAiPlayerNameIndices[playerInd - localPlayerCount]];
		}

		++playerInd;
	}

	return controller;
}

std::pair<wpc_c::iterator, wpc_c::iterator>
LocalPlayerController::get_wpc_choices_for_local_player_with_id(ID_t localPlayerID)
{
	return game.get_player_window_pattern_card_options(localPlayerID);
}

void LocalPlayerController::make_local_player_move_request(move_t move)
{
	game.move_request(move);
}

void LocalPlayerController::handle_local_player_WPC_choice(ID_t                localPlayerId,
                                                           const BoardContext& boardCtx)
{
	auto boardCtxClone = boardCtx.clone_without_dice();
	auto board         = std::make_shared<Board>(std::move(boardCtxClone));
	game.set_board(localPlayerId, board);
}

bool LocalPlayerController::is_local_player_id(ID_t id)
{
	return std::find(localPlayerIDs.begin(), localPlayerIDs.end(), id) != localPlayerIDs.end();
}

bool LocalPlayerController::local_player_on_move()
{
	return std::find(localPlayerIDs.begin(), localPlayerIDs.end(),
	                 game.get_curr_state()->player_on_move_id()) != localPlayerIDs.end();
}