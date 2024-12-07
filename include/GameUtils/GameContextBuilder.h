#ifndef GAME_CONTEXT_BUILDER_H
#define GAME_CONTEXT_BUILDER_H

#include <optional>

#include "BoardType.h"
#include "DiceConfig.h"
#include "GameContext.h"
#include "PlayerCountRelatedGameInformation.h"
#include "PublicObjectiveCardGameContext.h"
#include "PublicObjectiveCardType.h"
#include "RandomGenerator.h"
#include "ToolCardGameContext.h"
#include "ToolCardType.h"
#include "nlohmann/json.hpp"

/**
 * @brief Specific exception to prevent little bugs and
 * different errors occuring during the building process
 */
struct GameContextBuildingException : std::exception
{
  public:
    GameContextBuildingException(const std::string& _msg) : msg(_msg) {}

    const char* what() const noexcept override { return msg.c_str(); }

  private:
    std::string msg;
};

class GameContextBuilder;
using game_context_builder_t = std::unique_ptr<GameContextBuilder>;

/**
 * @brief Builder object for GameContexts
 *
 * The implementation makes sure that if the build function completes
 * successfully, the returned GameContext instance is surely correctly
 * configured.
 *
 * This implementation makes possible defining custom configurations for the
 * game. It is a very nice attribute if someone doesn't like a card or two and
 * wants to play without them.
 *
 * Implements the Builder design pattern.
 */
class GameContextBuilder
{
  public:
    /**
     * @brief Creates a Builder object
     * Receives the minimal information required for further configuration.
     * Checks if the configuration passed is correct and passes it to the
     * constructor.
     *
     * @param playerCountRelatedConfigs List of player count related information
     * that is defined for the given context
     * @param _puocContext The public objective card context used for the
     * configuration
     * @param _tcContext The tool card context used for the configuration
     * @param boardConfig The board configuration used for the configuration
     * @throws GameContextBuildingException If any of the parameters is invalid
     * e.g. the number of the cards exceeds the number of all cards
     * @return game_context_builder_t The builder object that is ready to be
     * configured via the functions
     */
    static game_context_builder_t create(
        const std::vector<PlayerCountRelatedGameInformation>&
            playerCountRelatedConfigs,
        puoc_context_t _puocContext, tc_context_t _tcContext,
        board_config_t boardConfig);

    /**
     * @brief Functions that allow to customize the final GameContexts
     *
     * @return GameContextBuilder& Always the same builder so it could be
     * chained
     */
    GameContextBuilder& set_name(const std::string& name);
    GameContextBuilder& add_selectable_wpc(wpc_c selectableWPC);
    GameContextBuilder& add_puoc(puoc_etype puocType);
    GameContextBuilder& add_tc(tc_etype tcType);
    GameContextBuilder& add_dice(dice_c _dice);
    GameContextBuilder& add_dice_config(dice_config_c diceConfig);
    GameContextBuilder& add_number_of_rounds(size_t _numberOfRounds);
    GameContextBuilder& add_wpc_choice_per_player(size_t _wpcChoicePerPlayer);
    GameContextBuilder& add_score_ctx(score_ctx_t scoreCtx);
    GameContextBuilder& add_player(player_t player, game_context_t& gameCtx);

    /**
     * @brief Build function that makes sure everything is well configured
     *
     * @param playerCount The number of players that will play the game
     * @param rnd Random object that will be used during the game
     * @throws GameContextBuildingException If any of the configurations is
     * invalid
     * @return game_context_t The GameContext object that is well-configured
     */
    game_context_t build(size_t playerCount, rnd_t rnd);

    const auto& get_name() { return name; }

  private:
    friend nlohmann::adl_serializer<GameContextBuilder>;

    /**
     * @brief Constructor for game context builders
     * Defined as private because the passed parameters may be invalid and those
     * configurations could be filtered out in the static create function.
     */
    GameContextBuilder(const std::vector<PlayerCountRelatedGameInformation>&
                           playerCountRelatedConfigs,
                       puoc_context_t _puocContext, tc_context_t _tcContext,
                       board_config_t boardConfig);

    std::vector<PlayerCountRelatedGameInformation> playerCountRelatedConfigs;

    /**
     * @brief Clones the GameContext that is already configured
     * Some of the attributes are directly stored in a GameContext object.
     * Since a builder should be used multiple times, these attributes are
     * cloned instead of moved.
     */
    game_context_t clone_underlying_context();

    game_context_t ctx;
    std::vector<puoc_etype> selectedPuocTypes;
    std::vector<tc_etype> selectedTcTypes;

    puoc_context_t puocContext;
    tc_context_t tcContext;
    board_config_t boardConfig;

    dice_c dice;
    dice_config_c diceConfig;
    size_t dicePerRound;
    std::optional<size_t> numberOfRounds;
    std::optional<size_t> wpcChoicePerPlayer;
    std::string name;

    /**
     * @brief Returns a color that is not attached to any player yet
     *
     * @param randomizedProcIndices The shuffled indices of colors
     * @param startingIndex Helper to check only unused dice
     * @param gameCtx Helper to have access to players -> already attached
     * colors
     * @return color_t A color that no previous player has as Private objective
     * card color
     */
    color_t get_unused_proc_color(std::vector<size_t>& randomizedProcIndices,
                                  size_t& startingIndex,
                                  game_context_t& gameCtx);

    dice_c clone_dice(rnd_t& rnd);

    /**
     * @brief Unified implementation for adding either a Tool card or a Public
     * objective card
     *
     * @tparam CardType tc_etype or puoc_etype
     * @param type
     * @param dest
     * @param cardCtx
     * @return GameContextBuilder&
     */
    template <typename CardType>
    GameContextBuilder& add_concrete_card(CardType type,
                                          std::vector<CardType>& dest,
                                          CardGameContext<CardType>* cardCtx)
    {
        if (dest.size() >= cardCtx->cards_per_game()) {
            throw GameContextBuildingException {
                "The limit for this type of cards has been already reached!"};
        }

        if (!cardCtx->contains(type)) {
            throw GameContextBuildingException {
                "This type of was not selected into the current game to play "
                "with!"};
        }

        dest.push_back(type);
        return *this;
    }

    /**
     * @brief Returns a random collection of required cards
     * Returns a copy for the same reason as the clone_underlying_context() is
     * used
     *
     * @tparam CardType tc_etype or puoc_etype
     * @param cardBuildingContainer Container that contains the fixed cards for
     * this context
     * @param cardContext Context describing the attributes of the cards
     * @param rnd Random configuration helping to randomize the card choosing
     * process
     * @return std::vector<CardType> Own copy of usable cards
     */
    template <typename CardType>
    std::vector<CardType> add_missing_cards(
        std::vector<CardType>& cardBuildingContainer,
        CardGameContext<CardType>& cardContext, rnd_t& rnd)
    {
        if (cardBuildingContainer.size() >= cardContext.cards_per_game())
            return cardBuildingContainer;

        std::vector<CardType> cardTypes = cardBuildingContainer;

        auto tmpCardTypeIndices =
            rnd->generate_random_id_order(cardContext.selected_card_count());

        for (auto&& cardInd : tmpCardTypeIndices) {
            CardType eType = cardContext.get_card_type_at_ind(cardInd);
            auto indIt = std::find(cardTypes.begin(), cardTypes.end(), eType);

            if (indIt != cardTypes.end()) {
                continue;
            }

            cardTypes.push_back(eType);

            if (cardTypes.size() == cardContext.cards_per_game()) {
                break;
            }
        }

        return cardTypes;
    }
};

#endif // GAME_CONTEXT_BUILDER_H