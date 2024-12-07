#ifndef CONTROLLER_WITH_AI_PLAYER_H
#define CONTROLLER_WITH_AI_PLAYER_H

#include "AI_Player.h"
#include "Game.h"

/**
 * @brief Base class for every controller that is responsible for managing the
 * communication in games
 *
 */
class ControllerWithAIPlayer
{
  public:
    ControllerWithAIPlayer(Game& _game) : game(_game) {}

    ai_player_t& get_next_ai_player()
    {
        return AI_PlayersByID[game.get_curr_state()->player_on_move_id()];
    }

    void make_next_ai_player_move()
    {
        move_t AI_PlayerMove = get_next_ai_player()->make_next_move();
        game.move_request(AI_PlayerMove);
    }

    Game& get_game() { return game; }
    auto& get_ai_players() { return AI_PlayersByID; }

    std::vector<ID_t> get_all_ids()
    {
        std::vector<ID_t> ids;
        for (auto&& player : game.get_ctx()->players) {
            ids.push_back(player->ID);
        }
        return ids;
    }

  protected:
    Game& game;
    std::unordered_map<ID_t, ai_player_t> AI_PlayersByID;
};

using ai_controller_t = std::unique_ptr<ControllerWithAIPlayer>;

#endif // CONTROLLER_WITH_AI_PLAYER_H