#include "Date.h"
#include "GameCacheInitializer.h"
#include "GameMode.h"
#include "LogConfig.h"
#include "Logger.h"
#include "SagradaGameCMDLineParser.h"
#include "SagradaWindow.h"
#include "ViewConstants.h"

#include <fstream>
#include <memory>

int main(int argc, char** argv)
{
	std::unique_ptr<std::ostream> logFileStream;
	SagradaCMDLineParser          program(logFileStream, "build/sagrada", "1.0",
	                                      argparse::default_arguments::help);
	SagradaGameCMDLineParser      simulationSubParser(logFileStream, false, &program, "simulation",
	                                                  "1.0", argparse::default_arguments::help);
	// SagradaGameCMDLineParser tournamentSubParser(
	//     logFileStream, true, &program, "tournament", "1.0",
	//     argparse::default_arguments::help);
	// SagradaGameCMDLineParser localPlaySubParser(
	//     logFileStream, false, &program, "localplay", "1.0",
	//     argparse::default_arguments::help);

	program.add_subparser(simulationSubParser);
	// program.add_subparser(tournamentSubParser);
	// program.add_subparser(localPlaySubParser);

	try {
		program.parse(argc, argv);
	}
	catch (const std::runtime_error& err) {
		exit_with_err_message(err.what(), program);
	}

	tournament_config_t tournamentConfig;
	game_mode_t         gameMode = game_mode_t::NONE;
	if (program.is_subcommand_used("simulation")) {
		tournamentConfig             = std::make_unique<TournamentConfig>();
		tournamentConfig->gameConfig = simulationSubParser.get_game_config();
		gameMode                     = game_mode_t::SIMULATION;
	}

	GameCacheInitializer::initialize_cache();
	auto app = Gtk::Application::create("org.sagrada");
	app->set_flags(Gio::ApplicationFlags::APPLICATION_NON_UNIQUE);
	auto window = std::make_unique<SagradaWindow>(v_constants::window::size, gameMode,
	                                              std::move(tournamentConfig));

	if (!window) return 1;

	return app->run(*window);
}