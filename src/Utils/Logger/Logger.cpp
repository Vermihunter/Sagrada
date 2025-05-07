////////////////////////////////////////
///        Vermes Ákos - 2022        ///
///                                  ///
///            Logger.cpp            ///
////////////////////////////////////////

#include "Logger.h"

/**
 * @brief Transforms the enum class value into its string value.
 * This container is used for printing the actual level of given
 * log message.
 */
const std::unordered_map<log_level, std::string> Logger::logStringByLevel{
    {log_level::TRACE, "TRACE"},  {log_level::DEBUG, "DEBUG"},  {log_level::INFO, "INFO"},
    {log_level::WARNING, "WARN"}, {log_level::ERROR_, "ERROR"}, {log_level::FATAL, "FATAL"}};