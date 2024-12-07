#ifndef GAME_CONFIG_READER_H
#define GAME_CONFIG_READER_H

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>

#include "AttributesJson.h"
#include "Constants.h"
#include "DiceConfig.h"
#include "GameCache.h"
#include "GameContextBuilder.h"
#include "PlayerCountRelatedGameInformation.h"
#include "Typedefs.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace fs = std::filesystem;

inline puoc_context_t get_puoc_ctx(json& j)
{
    size_t cardsPerGame = j.at("cards_per_game").get<size_t>();

    if (!j["puoc_contexts"].is_array()) {
        throw std::invalid_argument {"Invalid puoc config - contexts!"};
    }

    puoc_config_c puocConfig;
    for (auto&& json_obj : j.at("puoc_contexts")) {
        puocConfig.push_back(
            {json_obj.at("PuocId"),
             PublicObjectiveCardContext {
                 json_obj.at("SatisfactionValue").get<int>()}});
    }

    return std::make_unique<PublicObjectiveCardGameContext>(cardsPerGame,
                                                            puocConfig);
}

inline tc_context_t get_tc_ctx(json& j)
{
    if (!j.contains("cards_per_game") || !j.contains("selectable_ids")) {
        throw std::invalid_argument {"Invalid tc config - missing key!"};
    }

    size_t cardsPerGame = j.at("cards_per_game").get<size_t>();
    tc_config_c tcConfig = j.at("selectable_ids").get<tc_config_c>();

    return std::make_unique<ToolCardGameContext>(cardsPerGame, tcConfig);
}

inline BoardConfig* get_board_config(json& j)
{
    BoardType* type = BoardType::get_type(j.at("BoardType"));
    size_t width = j.at("Width");
    size_t height = j.at("Height");

    BoardConfig* config = BoardConfig::get_config(width, height, type);

    if (!config) {
        throw std::invalid_argument {"Invalid board config!"};
    }

    return config;
}

inline wpc_c get_selectable_wpc(json& j, board_config_t boardConfig)
{
    if (!j.is_array()) {
        throw std::invalid_argument {
            "WPC must be represented as an array of WPC names!"};
    }

    auto& allWpc = GameCache::get().get_wpc()[boardConfig];

    wpc_c res;
    for (auto&& wpcTitle : j) {
        auto wpcIt =
            std::find_if(allWpc.begin(), allWpc.end(), [&](const wpc_t& wpc) {
                return wpc->title == wpcTitle;
            });

        if (wpcIt == allWpc.end()) {
            throw std::invalid_argument {"Undefined WPC title: " +
                                         wpcTitle.dump()};
        }

        res.push_back(*wpcIt);
    }
    return res;
}

inline score_ctx_t get_score_ctx(json& j)
{
    int pointsPerUnusedFavorToken =
        j.at("pointsPerUnusedFavorToken").get<int>();
    int minusPointsPerEmptyField = j.at("minusPointsPerEmptyField").get<int>();
    int pointsPerProcValue = j.at("pointsPerProcValue").get<int>();

    return std::make_unique<ScoreContext>(pointsPerUnusedFavorToken,
                                          minusPointsPerEmptyField,
                                          pointsPerProcValue);
}

inline void from_json(const json& j, PlayerCountRelatedGameInformation& pi)
{
    j.at("PlayerCount").get_to(pi.playerCount);
    j.at("DiceCount").get_to(pi.diceCount);
}

inline dice_config_c get_dice_config(json& j)
{
    if (!j.is_array()) {
        throw std::invalid_argument {
            "Dice config should be an array consisting of dice configurations "
            "by each used color"};
    }

    dice_config_c diceConfig = std::make_unique<DiceConfig>();

    for (auto&& json_obj : j) {
        color_t diceColor = json_obj.at("Color").get<color_t>();

        diceConfig->diceByColor[diceColor].diceCount =
            json_obj.at("DiceCount").get<size_t>();
    }

    return diceConfig;
}

inline void process_json_config_file(const fs::path& path,
                                     game_ctx_builder_by_player_count_t& dest)
{
    std::ifstream ifs(path);
    json jf = json::parse(ifs);

    puoc_context_t puocCtx = get_puoc_ctx(jf.at("PuocConfig"));
    tc_context_t tcCtx = get_tc_ctx(jf.at("TcConfig"));
    board_config_t boardConfig = get_board_config(jf.at("BoardConfig"));
    auto playerRelatedInformation =
        jf.at("PlayerCountRelatedInformation")
            .get<std::vector<PlayerCountRelatedGameInformation>>();

    cached_game_ctx_builder_t gameCtxBuilder =
        GameContextBuilder::create(playerRelatedInformation, std::move(puocCtx),
                                   std::move(tcCtx), boardConfig);
    gameCtxBuilder->add_selectable_wpc(
        get_selectable_wpc(jf.at("SelectableWPC"), boardConfig));
    gameCtxBuilder->add_score_ctx(get_score_ctx(jf.at("ScoreContext")));
    gameCtxBuilder->add_wpc_choice_per_player(
        jf.at("wpcChoicePerPlayer").get<size_t>());
    gameCtxBuilder->add_number_of_rounds(jf.at("NumberOfRounds").get<size_t>());
    gameCtxBuilder->add_dice_config(get_dice_config(jf.at("DiceConfig")));
    gameCtxBuilder->set_name(path.stem().string());

    for (auto&& playerRelatedInfo : playerRelatedInformation) {
        auto playerCountCtxIt = dest.find(playerRelatedInfo.playerCount);
        if (playerCountCtxIt == dest.end()) {
            dest.insert({playerRelatedInfo.playerCount,
                         cached_game_ctx_builder_c {gameCtxBuilder}});
        }
        else {
            playerCountCtxIt->second.push_back(gameCtxBuilder);
        }
    }
}

/**
 * @brief Reads the defined game contexts from a defined config directory and
 * stores them in a container
 *
 */
class GameConfigReader
{
  public:
    static inline game_ctx_builder_by_player_count_t
    get_all_available_game_contexts()
    {
        game_ctx_builder_by_player_count_t allDefinedGameCtx;

        for (auto&& entry : fs::recursive_directory_iterator(gameConfigDir)) {
            if (entry.is_regular_file()) {
                if (entry.path().extension() != ".json") {
                    std::cerr << "Invalid config file found: " << entry.path()
                              << std::endl;
                }
                else {
                    try {
                        process_json_config_file(entry.path(),
                                                 allDefinedGameCtx);
                    }
                    catch (std::exception& e) {
                        std::cerr << "Invalid json config: " << entry.path()
                                  << " - " << e.what() << std::endl;
                    }
                }
            }
        }

        return allDefinedGameCtx;
    }
};

#endif // GAME_CONFIG_READER_H