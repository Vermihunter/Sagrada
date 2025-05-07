#include "GameInfo.h"

#include "Game.h"
#include "PlayerInformation.h"

const player_t& GameInfo::player_on_move(const Game& g)
{
	return get_player_with_id(g, g.get_curr_state()->player_on_move_id());
}

const player_t& GameInfo::get_player_with_id(const Game& g, ID_t playerId)
{
	return get_player_with_id_from_players(g.get_ctx()->players, playerId);
}

const player_t& GameInfo::get_player_with_id_from_players(const player_c& players, ID_t playerId)
{
	return players[playerId];
	/*
	auto playerIt = std::find_if(players.begin(), players.end(),
	    [&](const player_t& player) {
	        return player->ID == playerId;
	    });

	if(playerIt == players.end()) {
	    throw std::invalid_argument{"No player with the given ID exists"};
	}

	return *playerIt;
	*/
}