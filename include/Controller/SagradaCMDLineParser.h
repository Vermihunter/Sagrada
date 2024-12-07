#ifndef SAGRADA_CMD_LINE_PARSER_H
#define SAGRADA_CMD_LINE_PARSER_H

#include <argparse/argparse.hpp>
#include <fstream>
#include <memory>

#include "GameConfig.h"
#include "LogConfig.h"
#include "LogLevels.h"
#include "Logger.h"

/**
 * @brief Helper function that defines the log file name by the current date and
 * time to the default log dir
 *
 * @return std::string
 */
inline std::string generate_log_file_name()
{
    return DefaultLogSettings::logDir + "/" + Date::get_curr_date() + "-" +
           Date::get_curr_time() + ".log";
}

/**
 * @brief Helper function that prints an error message and exits with exit code
 * 1
 *
 */
inline /* [[noreturn]] */
    void
    exit_with_err_message(const std::string& errMessage,
                          argparse::ArgumentParser& parser)
{
    std::cerr << errMessage << std::endl;
    std::cerr << parser;
    std::exit(1);
}

/**
 * @brief Base command line parser that defines the options common for every
 * target
 *
 */
class SagradaCMDLineParser : public argparse::ArgumentParser
{
  public:
    virtual ~SagradaCMDLineParser() = default;

    template <typename... Args>
    SagradaCMDLineParser(std::unique_ptr<std::ostream>& _logFileStream,
                         Args&&... _args)
        : argparse::ArgumentParser(std::forward<Args>(_args)...)
        , logFileStream(_logFileStream)
    {
        add_argument("-v", "--verbose")
            .help("Prints additional information to the standard output")
            .default_value(false)
            .implicit_value(true);

        add_argument("-ll")
            .help("minimum log level - one of [TRACE, DEBUG, INFO, WARNING, "
                  "ERROR, FATAL]")
            .default_value(log_level::INFO)
            .action([&](const std::string& enteredLoglevel) {
                return log_level_from_str(enteredLoglevel);
            });

        add_argument("-ln").help(
            "log file name or \"cout\" and \"cerr\" values are accepted");
    }

    virtual void parse(int argc, char** argv)
    {
        parse_args(argc, argv);

        LogConfig logConfig;
        logConfig.minLogLevel = get<log_level>("-ll");

        if (auto logfilename = present("-ln")) {
            auto& value = logfilename.value();
            if (value == "cout") {
                logConfig.logStream = &std::cout;
            }
            else if (value == "cerr") {
                logConfig.logStream = &std::cerr;
            }
            else {
                fs::create_directories(fs::path(value).parent_path());
                logFileStream = std::make_unique<std::ofstream>(value);
                logConfig.logStream = logFileStream.get();
            }
        }
        else {
            fs::create_directory(DefaultLogSettings::logDir);
            logFileStream =
                std::make_unique<std::ofstream>(generate_log_file_name());
            logConfig.logStream = logFileStream.get();
        }

        Logger::get_instance().set_config(logConfig);
        if (logConfig.logStream->bad()) {
            std::cerr << "Bad log output stream" << std::endl;
            std::exit(1);
        }
    }

    const std::string& get_player_help() { return playerDefinitionHelp; }

  private:
    inline static const std::string playerDefinitionHelp =
        "The following AI players could be configured:\n"
        "- random\t\t\t\t\t\t\t- an agent choosing it's moves randomly\n"
        "  \n"
        "- first\t\t\t\t\t\t\t\t- an agent choosing the first possible move\n"
        "  \n"
        "- rules-based(-strategy=[STRATEGY])\t\t\t\t- an agent implementing a "
        "strategy based on "
        "human thinking. The following strategies could be chosen:\n"
        "  - all_moves - (default) the player takes all moves into "
        "consideration. Stronger "
        "strategy with speed performance payoff\n"
        "  - only_dtfm - the player always chooses a die to field move i.e. it "
        "doesn't check any "
        "of the Tool Card moves. Weaker strategy having greater speed "
        "performance\n"
        "  \n"
        "- "
        "minimax-depth=[UINT](,worlds=[UINT]|(,config_file=[PATH_TO_CONFIG."
        "json])\t- an agent "
        "implementing the minimax algorithm.\n"
        "  The worlds parameter determines the deterministic worlds created in "
        "each move making "
        "process. It has a default value of 4.\n"
        "  The value of the worlds parameter is ignored for deterministic "
        "games.\n"
        "  There is an option to configure the hyperparameters of the "
        "algorithm with a config "
        "file.\n"
        "  \n"
        "- "
        "mcts-it=[UINT](,expl=[DOUBLE])(,worlds=[UINT])(,playout=[PLAYOUT_"
        "STRATEGY])\t- an agent "
        "implementing the Monte Carlo tree search algoritm.\n"
        "  The worlds parameter determines the deterministic worlds created in "
        "each move making "
        "process. It has a default value of 4.\n"
        "  The value of the worlds parameter is ignored for deterministic "
        "games."
        "  The expl option makes possible to specify the exploration constant "
        "for the MCTS player. "
        "It accepts a double in the format [UINT].[UINT] or [UINT]"
        "  If the -d option is present, the value of the worlds parameter is "
        "ignored.\n"
        "  In the non-deterministic games, each world will be simulated in the "
        "given iteration "
        "count \n"
        "  i.e. for N worlds and I iterations, there will be N * I total "
        "iterations per move\n"
        "  The following playout strategies could be configured:\n"
        "  - random\t- This is the default strategy\n"
        "  - first\n"
        "  - minimax\t- this configuration uses the same syntax as the minimax "
        "player\n"
        "  - rules-based\n";

    std::unique_ptr<std::ostream>& logFileStream;
};

#endif // SAGRADA_CMD_LINE_PARSER_H