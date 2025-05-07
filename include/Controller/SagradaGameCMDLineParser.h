#ifndef SAGRADA_GAME_CMD_LINE_PARSER_H
#define SAGRADA_GAME_CMD_LINE_PARSER_H

#include "DefinedAIPlayers.h"
#include "GameConfig.h"
#include "SagradaCMDLineParser.h"

/**
 * @brief Command line parser that is used for components that start a game
 *
 */
class SagradaGameCMDLineParser : public SagradaCMDLineParser {
	static constexpr int  defaultSeed = 779;
	GameConfig            gameConfig;
	SagradaCMDLineParser* parent;

  public:
	template <typename... Args>
	SagradaGameCMDLineParser(std::unique_ptr<std::ostream>& _logFileStream, bool isTournamentParser,
	                         SagradaCMDLineParser* _parent, Args&&... _args)
	    : SagradaCMDLineParser(_logFileStream, std::forward<Args>(_args)...), parent(_parent)
	{
		add_argument("-s")
		    .help("The starting random seed for the tournament")
		    .default_value(defaultSeed)
		    .scan<'d', int>();

		auto& gameCountArg = add_argument("-n")
		                         .help("The number of games in the tournament")
		                         .scan<'u', unsigned int>();

		if (isTournamentParser) {
			gameCountArg.required();
		}

		add_argument("-d")
		    .help("Makes the game deterministic i.e. the players are familiar "
		          "with the upcoming dice "
		          "and the other players' Private objective card color.")
		    .nargs(0)
		    .default_value(false)
		    .implicit_value(true);

		add_argument("-p")
		    .help("The players that play the tournament")
		    .nargs(2)
		    .required()
		    .action([&](std::string s) {
			    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
			    ai_player_config_t config = DefinedAIPlayers::construct_by_name(s);
			    if (!config) {
				    throw std::runtime_error{"Invalid AI player type"};
			    }
			    gameConfig.players.push_back(std::move(config));
		    });

		add_argument("-mode")
		    .help("The game mode to play with")
		    .default_value(std::string("default"));
	}

	game_config_t get_game_config();
};

#endif // SAGRADA_GAME_CMD_LINE_PARSER_H