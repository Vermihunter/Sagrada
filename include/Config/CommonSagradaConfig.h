#ifndef SAGRADA_CONFIG_H
#define SAGRADA_CONFIG_H

#include <optional>
#include <string>

#include "LogLevels.h"

/**
 * @brief Basic configuration that every target contains
 *
 */
struct SagradaConfig
{
    std::optional<std::string> logFileName;
    log_level minLogLevel;
};

#endif // SAGRADA_CONFIG_H