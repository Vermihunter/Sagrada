#ifndef DEFAULT_GAME_CREATOR_H
#define DEFAULT_GAME_CREATOR_H

#include <memory>
#include <vector>

#include "DefaultGameConstants.h"
#include "DiceConfig.h"
#include "DiceSupply.h"
#include "Game.h"
#include "GameContext.h"
#include "GameContextBuilder.h"
#include "PlayerCountRelatedGameInformation.h"
#include "PublicObjectiveCardGameContext.h"
#include "PublicObjectiveCardType.h"
#include "RandomGenerator.h"
#include "ToolCardGameContext.h"

/**
 * @brief Creates a Game according to the official rules of Sagrada
 *
 * Uses the GameContextBuilder interface configured by the constants defined in
 * DefaultGameContants.h
 */
struct DefaultGameCreator
{
    static std::unique_ptr<Game> create(size_t playerCount, rnd_t rnd)
    {
        if (playerCount > defaultMaxPlayers) {
            throw std::invalid_argument {"The maximum player count is " +
                                         std::to_string(playerCount)};
        }

        return std::make_unique<Game>(
            get_default_context(playerCount, std::move(rnd)),
            get_default_game_state());
    }

    static game_state_t get_default_game_state()
    {
        game_state_t defaultGameState = std::make_unique<GameState>();
        defaultGameState->hasEnded = false;
        defaultGameState->roundNumber = 0;
        defaultGameState->playerStartedThisRoundIndex = 0;
        defaultGameState->currPlayerIndex = 0;
        defaultGameState->previousToolCardMove = nullptr;
        defaultGameState->currRoundInfo = nullptr;
        defaultGameState->previousToolCardMove = 0;
        return defaultGameState;
    }

    static auto get_default_player_count_related_config()
    {
        return std::vector<PlayerCountRelatedGameInformation> {
            PlayerCountRelatedGameInformation {.playerCount = 2,
                                               .diceCount = 5},
            PlayerCountRelatedGameInformation {.playerCount = 3,
                                               .diceCount = 7},
            PlayerCountRelatedGameInformation {.playerCount = 4,
                                               .diceCount = 9}};
    }

    static game_context_t get_default_context(size_t playerCount, rnd_t rnd)
    {
        board_config_t defaultBoardConfig =
            BoardConfig::get_config(5, 4, BoardType::get_type("Rectangle"));
        return GameContextBuilder::create(
                   get_default_player_count_related_config(),
                   get_default_puoc_context(), get_default_tc_context(),
                   defaultBoardConfig)
            ->add_dice_config(get_default_dice_config())
            .set_name("default")
            .add_number_of_rounds(defaultNumberOfRounds)
            .add_wpc_choice_per_player(defaultWPCChoicePerPlayer)
            .add_score_ctx(get_default_score_ctx())
            .build(playerCount, std::move(rnd));
    }

    static score_ctx_t get_default_score_ctx()
    {
        return std::make_unique<ScoreContext>(
            /* pointsPerUnusedFavorToken = */ 1,
            /* minusPointsPerEmptyField = */ 1,
            /* pointsPerProcValue = */ 1);
    }

    static dice_config_c get_default_dice_config()
    {
        dice_config_c diceConfig = std::make_unique<DiceConfig>();

        for (size_t i = 0; i < size_t(color_t::COUNT); ++i) {
            diceConfig->diceByColor[color_t(i)].diceCount = defaultDicePerColor;
        }

        return diceConfig;
    }

    static tc_context_t get_default_tc_context()
    {
        tc_config_c tcConfig;
        for (size_t i = 0; i < size_t(tc_etype::COUNT); ++i) {
            tcConfig.push_back(tc_etype(i));
        }

        return std::make_unique<ToolCardGameContext>(
            /* cardsPerGame = */ defaultTcPerGame,
            /* selectedCards = */ tcConfig);
    }

    static puoc_context_t get_default_puoc_context()
    {
        puoc_config_c puocConfig {{puoc_etype::UNIQUE_VALUE_ROW, 5},
                                  {puoc_etype::UNIQUE_COLOR_ROW, 6},
                                  {puoc_etype::UNIQUE_VALUE_COLUMN, 4},
                                  {puoc_etype::UNIQUE_COLOR_COLUMN, 5},
                                  {puoc_etype::ONE_AND_TWO_COLLECTING, 2},
                                  {puoc_etype::THREE_AND_FOUR_COLLECTING, 2},
                                  {puoc_etype::FIVE_AND_SIX_COLLECTING, 2},
                                  {puoc_etype::ALL_VALUE_COLLECTING, 5},
                                  {puoc_etype::ALL_COLOR_COLLECTING, 4},
                                  {puoc_etype::DIAGONAL_CONNECTING, 1}};

        return std::make_unique<PublicObjectiveCardGameContext>(
            /* cardsPerGame = */ defaultPuocPerGame,
            /* selectecCards = */ puocConfig);
    }
};

#endif // DEFAULT_GAME_CREATOR_H