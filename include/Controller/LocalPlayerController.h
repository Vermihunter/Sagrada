#ifndef LOCAL_PLAYER_CONTROLLER_H
#define LOCAL_PLAYER_CONTROLLER_H

#include "ControllerWithAIPlayer.h"

class LocalPlayerController;
using local_player_controller_t = std::unique_ptr<LocalPlayerController>;

/**
 * @brief Controller implementation that is used when there is at least one
 * local player
 *
 */
class LocalPlayerController : public ControllerWithAIPlayer {
  public:
	/**
	 * @brief The only publically available Factory constructor
	 *
	 * @param localPlayerCount Number of local players to play
	 * @param configs AI player configurations that play
	 * @param game Game state context
	 * @param gameDeterministic Flag whether the game is deterministic or not
	 * (must be passed to AI players)
	 * @return local_player_controller_t The resulting controller
	 */
	static local_player_controller_t with_players(size_t                    localPlayerCount,
	                                              const ai_player_config_c& configs, Game& game,
	                                              const bool gameDeterministic = false);

	std::pair<wpc_c::iterator, wpc_c::iterator>
	get_wpc_choices_for_local_player_with_id(ID_t localPlayerID);

	void make_local_player_move_request(move_t move);
	void handle_local_player_WPC_choice(ID_t localPlayerId, const BoardContext& boardCtx);

	bool        is_local_player_id(ID_t id);
	bool        local_player_on_move();
	const auto& get_local_player_ids() { return localPlayerIDs; }

  private:
	LocalPlayerController(Game& _game) : ControllerWithAIPlayer(_game) {}

	std::vector<ID_t> localPlayerIDs;
};

#endif // LOCAL_PLAYER_CONTROLLER_H