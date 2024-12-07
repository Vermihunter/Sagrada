#include "DefinedAIPlayers.h"

#include <regex>
#include <string_view>

#include "FirstPlayerConfig.h"
#include "MCTSPlayerConfig.h"
#include "MCTSPlayerDefaultConfig.h"
#include "MinimaxPlayerConfig.h"
#include "RandomPlayerConfig.h"
#include "RulesBasedPlayerConfig.h"

auto to_ull(const std::string& str)
{
    size_t pos;

    size_t res = std::stoull(str, &pos);
    if (pos != str.size()) {
        throw std::runtime_error {"The it parameter must be a number"};
    }

    return res;
}

DefinedAIPlayers::DefinedAIPlayers()
{
    definedPlayerConfigs.push_back(std::make_unique<RandomPlayerConfig>());
    definedPlayerConfigs.push_back(std::make_unique<FirstPlayerConfig>());
    definedPlayerConfigs.push_back(std::make_unique<MinimaxPlayerConfig>(
        2, defaultConstants, MINIMAX_defaultWorldCount));
    definedPlayerConfigs.push_back(std::make_unique<RulesBasedPlayerConfig>(
        RulesBasedPlayer::strategy_t::ALL_MOVES));

    auto mcts_ctx = std::make_unique<MCTSPlayerContext>();
    mcts_ctx->playoutStrategy = MCTS_defaultPlayoutStrategy->clone();
    mcts_ctx->iterationCount = 120;
    mcts_ctx->determinizingWorldCount = 3;
    definedPlayerConfigs.push_back(
        std::make_unique<MCTSPlayerConfig>(std::move(mcts_ctx)));
}

ai_player_config_t DefinedAIPlayers::construct_by_name(
    const std::string& aiName)
{
    if (aiName == "random") {
        return std::make_unique<RandomPlayerConfig>();
    }
    else if (aiName == "first") {
        return std::make_unique<FirstPlayerConfig>();
    }
    else if (aiName.find("rules-based") == 0 && aiName.size() >= 11) {
        std::regex pattern("^rules-based(?:-strategy=([^,]+))?$");

        // Check if the input strings match the pattern
        std::smatch match;

        // The name did not match the regex
        if (!std::regex_match(aiName, match, pattern)) {
            return nullptr;
        }

        RulesBasedPlayer::strategy_t strategy =
            RulesBasedPlayer::DEFAULT_STRATEGY;
        if (match[1].length() > 0) {
            if (match[1] == "all_moves") {
                strategy = RulesBasedPlayer::strategy_t::ALL_MOVES;
            }
            else if (match[1] == "only_dtfm") {
                strategy = RulesBasedPlayer::strategy_t::ONLY_DTFM;
            }
            else {
                return nullptr;
            }
        }

        return std::make_unique<RulesBasedPlayerConfig>(strategy);
    }
    // Minimax format: minimax-depth=3
    else if (aiName.find("minimax") == 0 && aiName.size() >= 15) {
        HeuristicConstants heuristicConstants = defaultConstants;
        // Define the regex pattern
        std::regex pattern(
            "^minimax-depth=(\\d+)(,worlds=(\\d+))?(?:,config_file=([^,]+))?$");

        // Check if the input strings match the pattern
        std::smatch match;

        // The name did not match the regex
        if (!std::regex_match(aiName, match, pattern)) {
            return nullptr;
        }

        if (match[4].length() > 0) {
            std::ifstream stream(match[4]);
            json config_json;
            stream >> config_json;
            heuristicConstants = config_json;
        }

        size_t depth = to_ull(match[1]);

        int worldCount = -1;
        if (match[3].length() > 0) {
            worldCount = int(to_ull(match[3]));
        }

        return std::make_unique<MinimaxPlayerConfig>(depth, heuristicConstants,
                                                     worldCount);
    }
    else if (aiName.find("mcts-it") == 0 && aiName.size() >= 9) {
        // Define the regex pattern
        std::regex pattern("^mcts-it=(\\d+)(,expl=(\\d+(\\.\\d+)?))?(,worlds=("
                           "\\d+))?(?:,playout=(.+))?$");

        // Check if the input strings match the pattern
        std::smatch match;

        // The name did not match the regex
        if (!std::regex_match(aiName, match, pattern)) {
            return nullptr;
        }

        mcts_context_t ctx = std::make_unique<MCTSPlayerContext>();
        ctx->iterationCount = to_ull(match[1]);

        if (match[3].length() > 0) {
            ctx->explorationConstant = std::stod(match[3]);
        }

        if (match[5].length() > 0) {
            ctx->determinizingWorldCount = to_ull(match[6]);
        }

        if (match[7].length() > 0) {
            ctx->playoutStrategy = construct_by_name(match[7]);
            if (!ctx->playoutStrategy) {
                return nullptr;
            }
        }
        else {
            ctx->playoutStrategy = MCTS_defaultPlayoutStrategy->clone();
        }

        return std::make_unique<MCTSPlayerConfig>(std::move(ctx));
    }
    else {
        return nullptr;
    }
}