#include "AI_Player.h"

AI_Player::AI_Player(const AIPlayerContext& _ctx) : ctx(_ctx) {}

ai_player_t AI_Player::initialize_player(Game& game,
                                         const AI_PlayerConfig& config,
                                         const player_t& playerInfo,
                                         const bool _gameDeterministic)
{
    AIPlayerContext _ctx {.ID = playerInfo->ID,
                          .game = game,
                          .privateObjectiveCard = playerInfo->PrOC,
                          .gameDeterministic = _gameDeterministic};

    ai_player_t newAIPlayer = config.create_player(_ctx);

    auto [wpc_begin, wpc_end] =
        game.get_player_window_pattern_card_options(playerInfo->ID);
    ID_t chosenWPCIndex =
        newAIPlayer->choose_window_pattern_card(wpc_begin, wpc_end);

    std::advance(wpc_begin, chosenWPCIndex);

    auto boardCtx = (*wpc_begin)->clone_without_dice();
    auto boardFromWPC = std::make_shared<Board>(std::move(boardCtx));
    game.set_board(playerInfo->ID, boardFromWPC);

    return newAIPlayer;
}