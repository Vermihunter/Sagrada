#include "Tournament.h"

#include <algorithm>
#include <vector>

#include "GameCache.h"
#include "ScoreLog.h"

std::vector<std::vector<size_t>> get_all_permutations(size_t N)
{
    std::vector<size_t> numbers(N);
    for (size_t i = 0; i < N; ++i) {
        numbers[i] = i;
    }

    std::vector<std::vector<size_t>> result;

    do
        result.push_back(numbers);
    while (std::next_permutation(numbers.begin(), numbers.end()));

    return result;
}

Tournament::Tournament(tournament_config_t _config,
                       TournamentStatistics& _destStatistics)
    : config(std::move(_config)), statistics(_destStatistics)
{
}

TournamentStatistics Tournament::run(tournament_config_t config)
{
    TournamentStatistics statistics {config->gameConfig->players};

    std::vector<ai_player_config_pc> aiConfigs;
    if (config->allDirections) {
        std::vector<std::vector<size_t>> allAIPermutations =
            get_all_permutations(config->gameConfig->players.size());

        for (auto&& permutation : allAIPermutations) {
            aiConfigs.push_back(ai_player_config_pc {});
            ai_player_config_pc& lastPermutation = aiConfigs.back();
            for (auto&& permutationInd : permutation) {
                lastPermutation.push_back(
                    config->gameConfig->players[permutationInd].get());
            }
        }
    }
    else {
        ai_player_config_pc onlyConfig;
        for (auto&& player : config->gameConfig->players) {
            onlyConfig.push_back(player.get());
        }
        aiConfigs.push_back(onlyConfig);
    }

    Tournament model {std::move(config), statistics};

    int randomSeed = model.config->gameConfig->randomSeed;
    for (size_t i = 0; i < size_t(model.config->gameCount) / aiConfigs.size();
         ++i) {
        model.launch_games(randomSeed++, i, aiConfigs);
    }

    for (auto&& printer : model.config->tournamentPrinters) {
        printer->print_overall_statistics(statistics);
    }

    return statistics;
}

tournament_game_info_t Tournament::launch_game(int seed, size_t gameNumber,
                                               ai_player_config_pc& aiConfigs)
{
    auto gameCtx = config->gameConfig->gameCtxBuilder->build(
        config->gameConfig->players.size(),
        RandomGenerator::initialize_with_seed(seed));

    tournament_game_info_t gameInformation =
        TournamentGameInformation::create(aiConfigs);
    gameInformation->game = std::make_unique<Game>(
        std::move(gameCtx), DefaultGameCreator::get_default_game_state());
    gameInformation->gameInfo->randomSeed = seed;
    gameInformation->gameInfo->gameCtxBuilder =
        config->gameConfig->gameCtxBuilder;

    gameInformation->controller = std::make_unique<OnlyAIPlayerController>(
        *gameInformation->game, aiConfigs,
        config->gameConfig->gameDeterministic);
    auto& game = gameInformation->controller->get_game();
    auto& AI_players = gameInformation->controller->get_ai_players();
    auto& playerToConfig =
        gameInformation->controller->get_player_to_config_mapper();

    while (!game.get_curr_state()->hasEnded) {
        ID_t playerOnMoveId = game.get_curr_state()->player_on_move_id();
        auto playerConfig =
            playerToConfig.at(AI_players.at(playerOnMoveId).get());
        auto playerStatIt = statistics.playerStatistics.find(playerConfig);

        if (config->saveMoveStatistics) {
            for (auto&& tournamentPrinter : config->tournamentPrinters) {
                tournamentPrinter->print_move_statistics(
                    gameInformation->controller->get_next_ai_player()
                        ->get_possible_moves(game),
                    game, gameNumber, playerConfig);
            }
        }

        auto start_time = std::chrono::high_resolution_clock::now();

        gameInformation->controller->make_next_ai_player_move();

        auto end_time = std::chrono::high_resolution_clock::now();

        playerStatIt->second.totalMoveTimeInMs +=
            std::chrono::duration_cast<std::chrono::milliseconds>(end_time -
                                                                  start_time)
                .count();
        ++playerStatIt->second.totalMoveCount;
    }
    gameInformation->finalScores = game.evaluate();

    return gameInformation;
}

std::vector<tournament_game_info_t> Tournament::launch_games(
    int seed, size_t gameGroupCount,
    std::vector<ai_player_config_pc>& aiConfigs)
{
    std::vector<tournament_game_info_t> results;
    size_t ind = 0;
    for (auto&& aiConfig : aiConfigs) {
        results.push_back(launch_game(
            seed, gameGroupCount * aiConfigs.size() + ind, aiConfig));
        auto gameResult = update_statistics(results.back());
        print_results(results.back(), gameResult, seed);

        statistics.gameResults.push_back(std::move(gameResult));
        ++ind;
    }

    return results;
}

game_result_t Tournament::update_statistics(
    tournament_game_info_t& gameInformation)
{
    gameInformation->finalScores = gameInformation->game->evaluate();
    for (auto&& score : gameInformation->finalScores) {
        LOG_SCORE(*score);
    }

    auto& AI_players = gameInformation->controller->get_ai_players();
    auto& playerToConfig =
        gameInformation->controller->get_player_to_config_mapper();

    game_result_t gameResult = std::make_unique<GameResult>();

    for (auto&& [player, playerConfig] :
         /*config->gameConfig->players*/ playerToConfig) {
        // auto playerConfig = playerC.get();
        auto& playerScore = *std::find_if(
            gameInformation->finalScores.begin(),
            gameInformation->finalScores.end(), [&](const eval_state_t& score) {
                auto& l_player = score->get_player();

                auto& aiPlayerIt = AI_players[l_player->ID];
                // auto scorePlayerConfig =
                // playerToConfig.at(AI_players.at(l_player
                // ->ID).get());
                auto scorePlayerConfig = playerToConfig.at(aiPlayerIt.get());
                return scorePlayerConfig == playerConfig;
            });

        auto playerIt = statistics.playerStatistics.find(playerConfig);

        auto& playerStatistics = playerIt->second;
        playerStatistics.totalReceivedPoints += playerScore->get_total_points();

        gameResult->pointsByPlayers.push_back(
            {playerConfig, playerScore->get_total_points()});
    }

    ai_player_t& winnerPlayer =
        AI_players[gameInformation->finalScores[0]->get_player()->ID];
    AI_PlayerConfig* winnerConfig = playerToConfig[winnerPlayer.get()];

    ++statistics.playerStatistics.find(winnerConfig)->second.winCount;
    gameResult->winner = winnerConfig;

    return gameResult;
}

void Tournament::print_results(tournament_game_info_t& gameInfo,
                               game_result_t& gameResult, int seed)
{
    for (auto&& printer : config->tournamentPrinters) {
        printer->print_game_result(*gameInfo, gameResult, seed);
    }
}
