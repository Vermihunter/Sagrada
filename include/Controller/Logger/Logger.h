////////////////////////////////////////
///         Vermes �kos � 2022       ///
///                                  ///
///             Logger.h             ///
////////////////////////////////////////

#ifndef LOGGER_H_
#define LOGGER_H_

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_map>

#include "Date.h"
#include "LogConfig.h"
#include "LogLevels.h"

template <typename T>
concept HasOutputOperator =
    requires(std::ostream& os, const T& obj) { os << obj; };

namespace fs = std::filesystem;

/**
 * @brief Logger interface used for logging with different levels.
 *
 * Each function in the class is static because this way,
 * passing an instance between classes or constructing an instance
 * before each logging is avoided which in my opinion would be
 * really disgusting.
 *
 * There are defined log levels in the enum class LOG, the user
 * can define what log level will be used and anything that is
 * "greater" than the defined level will be printed to logfile.
 *
 */
class Logger
{
  public:
    ~Logger() {}

    static auto& get_instance()
    {
        static Logger logger;
        return logger;
    }

    template <HasOutputOperator T>
    void log(log_level level, const T& msg)
    {
        /** The log message has lower priority than the minimum defined*/
        if (config.minLogLevel > level)
            return;

        if (config.logStream->bad()) {
            std::cerr << "Huge problems with Logger output stream" << std::endl;
        }

        /** Log format */
        (*config.logStream)
            << "[ " << logStringByLevel.at(level) << " - "
            << Date::get_curr_date() << " - " << Date::get_curr_time()
            << " ] - " << msg << std::endl;
    }

    void set_config(const LogConfig& _config) { config = _config; }

    /** Container for transforming to string */
    static const std::unordered_map<log_level, std::string> logStringByLevel;

  private:
    Logger()
    {
        config.minLogLevel = DefaultLogSettings::minLogLevel;
        config.logStream = DefaultLogSettings::logStream;
    }

    LogConfig config;
};

inline log_level log_level_from_str(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    auto it = std::find_if(
        Logger::logStringByLevel.begin(), Logger::logStringByLevel.end(),
        [&](const auto& logInfo) { return logInfo.second == str; });

    if (it == Logger::logStringByLevel.end()) {
        throw std::runtime_error {"Invalid log level: '" + str + "'"};
    }

    return it->first;
}

/** Global functions to make the API cleaner */
template <HasOutputOperator T>
inline void LOG_T(const T& logMsg)
{
    Logger::get_instance().log(log_level::TRACE, logMsg);
}

template <HasOutputOperator T>
inline void LOG_D(const T& logMsg)
{
    Logger::get_instance().log(log_level::DEBUG, logMsg);
}

template <HasOutputOperator T>
inline void LOG_I(const T& logMsg)
{
    Logger::get_instance().log(log_level::INFO, logMsg);
}

template <HasOutputOperator T>
inline void LOG_W(const T& logMsg)
{
    Logger::get_instance().log(log_level::WARNING, logMsg);
}

template <HasOutputOperator T>
inline void LOG_E(const T& logMsg)
{
    Logger::get_instance().log(log_level::ERROR_, logMsg);
}

template <HasOutputOperator T>
inline void LOG_F(const T& logMsg)
{
    Logger::get_instance().log(log_level::FATAL, logMsg);
}

#endif
