#include "GameContextBuilder.h"

#include "DefaultGameCreator.h"
#include "GameCache.h"

game_context_builder_t GameContextBuilder::create(
    const std::vector<PlayerCountRelatedGameInformation>& playerCountRelatedConfigs,
    puoc_context_t _puocContext, tc_context_t _tcContext,
    board_config_t boardConfig)
{
    if (_puocContext->cards_per_game() > _puocContext->get_current_game_puocs().size()) {
        throw GameContextBuildingException {
            "There are less different card configurations than the required "
            "amount of cards in the game!"};
    }

    if (_tcContext->cards_per_game() >
        _tcContext->get_current_game_tcs().size()) {
        throw GameContextBuildingException {
            "There are less different card configurations than the required "
            "amount of cards in the "
            "game!"};
    }

    if (playerCountRelatedConfigs.empty()) {
        throw GameContextBuildingException {
            "No player count related config provided!"};
    }

    GameContextBuilder* tmpRawBuilder = new GameContextBuilder(
        playerCountRelatedConfigs, std::move(_puocContext),
        std::move(_tcContext), boardConfig);
    return std::unique_ptr<GameContextBuilder>(tmpRawBuilder);
}

GameContextBuilder::GameContextBuilder(
    const std::vector<PlayerCountRelatedGameInformation>&
        _playerCountRelatedConfigs,
    puoc_context_t _puocContext, tc_context_t _tcContext,
    board_config_t _boardConfig)
    : playerCountRelatedConfigs(_playerCountRelatedConfigs)
    , puocContext(std::move(_puocContext))
    , tcContext(std::move(_tcContext))
    , boardConfig(_boardConfig)

{
    GameContext* rawCtx = new GameContext();
    ctx = std::unique_ptr<GameContext>(rawCtx);
}

/*
GameContextBuilder&
GameContextBuilder::add_basic_move_handler(std::unique_ptr<BasicMoveHandler>
_basicMoveHandler) { ctx->basicMoveHandler = std::move(_basicMoveHandler);
return *this;
}
*/
GameContextBuilder& GameContextBuilder::set_name(const std::string& _name)
{
    name = _name;
    return *this;
}

GameContextBuilder& GameContextBuilder::add_selectable_wpc(wpc_c _selectableWPC)
{
    ctx->selectableWPC = _selectableWPC;
    return *this;
}

GameContextBuilder& GameContextBuilder::add_puoc(puoc_etype puocType)
{
    return add_concrete_card(puocType, selectedPuocTypes, puocContext.get());
}

GameContextBuilder& GameContextBuilder::add_tc(tc_etype tcType)
{
    return add_concrete_card(tcType, selectedTcTypes, tcContext.get());
}

GameContextBuilder& GameContextBuilder::add_dice(dice_c _dice)
{
    dice = std::move(_dice);
    return *this;
}

GameContextBuilder& GameContextBuilder::add_dice_config(
    dice_config_c _diceConfig)
{
    diceConfig = std::move(_diceConfig);
    return *this;
}

GameContextBuilder& GameContextBuilder::add_number_of_rounds(
    size_t _numberOfRounds)
{
    numberOfRounds = _numberOfRounds;
    return *this;
}

GameContextBuilder& GameContextBuilder::add_wpc_choice_per_player(
    size_t _wpcChoicePerPlayer)
{
    wpcChoicePerPlayer = _wpcChoicePerPlayer;
    return *this;
}

GameContextBuilder& GameContextBuilder::add_player(player_t player,
                                                   game_context_t& gameCtx)
{
    if (gameCtx->players.size() >= size_t(color_t::COUNT)) {
        throw GameContextBuildingException {
            "There are not enough colors for each player to have a unique "
            "Private objective card!"};
    }

    bool confictingPlayerAlreadyExists =
        std::any_of(gameCtx->players.begin(), gameCtx->players.end(),
                    [&](const player_t& alreadyAddedPlayer) {
                        return alreadyAddedPlayer->ID == player->ID ||
                               alreadyAddedPlayer->PrOC->get_color() ==
                                   player->PrOC->get_color();
                    });

    if (confictingPlayerAlreadyExists) {
        throw std::invalid_argument {"A player with conflicting configuration "
                                     "already exists in the Game!"};
    }

    gameCtx->players.push_back(std::move(player));

    return *this;
}

GameContextBuilder& GameContextBuilder::add_score_ctx(score_ctx_t scoreCtx)
{
    ctx->scoreCtx = std::move(scoreCtx);
    return *this;
}

game_context_t GameContextBuilder::clone_underlying_context()
{
    auto newContext = std::unique_ptr<GameContext>(new GameContext());
    newContext->selectableWPC = ctx->selectableWPC;
    newContext->numberOfRounds = ctx->numberOfRounds;
    newContext->dicePerRound = ctx->dicePerRound;
    newContext->playerWpcChoiceCount = ctx->playerWpcChoiceCount;

    if (ctx->scoreCtx) {
        newContext->scoreCtx = ctx->scoreCtx->clone();
    }

    if (ctx->basicMoveHandler) {
        newContext->basicMoveHandler = ctx->basicMoveHandler->clone();
    }

    return newContext;
}

game_context_t GameContextBuilder::build(size_t playerCount, rnd_t rnd)
{
    auto configByPlayerCount = [&](const auto& countRelatedConfig) {
        return countRelatedConfig.playerCount == playerCount;
    };

    auto playerCountRelatedConfigIt =
        std::find_if(playerCountRelatedConfigs.begin(),
                     playerCountRelatedConfigs.end(), configByPlayerCount);
    if (playerCountRelatedConfigIt == playerCountRelatedConfigs.end()) {
        throw GameContextBuildingException {
            "No config provided for the given player count!"};
    }

    auto tmpCtx = clone_underlying_context();

    if (playerCount > size_t(color_t::COUNT)) {
        throw GameContextBuildingException {
            "There are not enough colors for each player to have a unique "
            "Private objective card!"};
    }

    std::vector<puoc_etype> puocTypes =
        add_missing_cards(selectedPuocTypes, *puocContext, rnd);
    std::vector<tc_etype> tcTypes =
        add_missing_cards(selectedTcTypes, *tcContext, rnd);

    if (tmpCtx->selectableWPC.empty()) {
        tmpCtx->selectableWPC = GameCache::get().get_wpc()[boardConfig];
    }
    rnd->shuffle(tmpCtx->selectableWPC);

    if (!dice && !diceConfig) {
        add_dice_config(DefaultGameCreator::get_default_dice_config());
    }

    tmpCtx->supply = std::make_unique<DiceSupply>(2 * playerCount + 1,
                                                  clone_dice(rnd), *rnd);

    tmpCtx->numberOfRounds = numberOfRounds.has_value() ? numberOfRounds.value()
                                                        : defaultNumberOfRounds;
    tmpCtx->dicePerRound = playerCountRelatedConfigIt->diceCount;
    tmpCtx->playerWpcChoiceCount = wpcChoicePerPlayer.has_value()
                                       ? wpcChoicePerPlayer.value()
                                       : defaultWPCChoicePerPlayer;

    for (auto&& puoc_type : puocTypes) {
        tmpCtx->publicObjectiveCards.push_back(
            PublicObjectiveCard::create_by_type(
                puoc_type, puocContext->get_context_for(puoc_type)));
    }

    for (auto&& tc_type : tcTypes) {
        tmpCtx->toolCards.push_back(ToolCard::create(tc_type));
    }

    auto procIndices = rnd->generate_random_id_order(size_t(color_t::COUNT));
    // players
    size_t procIndex = 0;
    for (size_t i = 0; i < playerCount; ++i) {
        color_t procColor =
            get_unused_proc_color(procIndices, procIndex, tmpCtx);
        proc_t newPlayerProc = PrivateObjectiveCard::construct(procColor);

        add_player(std::make_unique<PlayerInformation>(i, "", newPlayerProc),
                   tmpCtx);
    }

    if (!tmpCtx->basicMoveHandler) {
        tmpCtx->basicMoveHandler =
            std::make_unique<BasicMoveHandler>(boardConfig);
    }

    if (!tmpCtx->scoreCtx) {
        tmpCtx->scoreCtx = DefaultGameCreator::get_default_score_ctx();
    }

    tmpCtx->rnd = std::move(rnd);

    return tmpCtx;
}

dice_c GameContextBuilder::clone_dice(rnd_t& rnd)
{
    // If the builder is configured with concrete dice, those dice are used
    if (dice) {
        return std::make_unique<std::vector<Die>>(*dice);
    }

    if (!diceConfig) {
        throw std::invalid_argument {"No dice and no dice config found!"};
    }

    // Otherwise, the user could choose a dice config (selecting the number of
    // dice for each color), and use this config as a base config for rerolling
    // each round
    auto tmpDice = std::make_unique<std::vector<Die>>();
    for (auto&& [dieColor, oneColoredDiceConfig] : diceConfig->diceByColor) {
        for (size_t i = 0; i < oneColoredDiceConfig.diceCount; ++i) {
            tmpDice->push_back(Die {dieColor, rnd->roll_die()});
        }
    }
    rnd->shuffle(*tmpDice);

    return tmpDice;
}

color_t GameContextBuilder::get_unused_proc_color(
    std::vector<size_t>& randomizedProcIndices, size_t& startingIndex,
    game_context_t& gameCtx)
{
    for (size_t i = startingIndex; i < randomizedProcIndices.size(); ++i) {
        color_t newColor = color_t(randomizedProcIndices[i]);

        if (std::none_of(gameCtx->players.begin(), gameCtx->players.end(),
                         [&](const player_t& player) {
                             return player->PrOC->get_color() == newColor;
                         })) {
            return newColor;
        }
    }

    throw std::invalid_argument {"All the colors have been already used as "
                                 "Private objective card colors!"};
}