#ifndef SAGRADA_CONFIG_H
#define SAGRADA_CONFIG_H

#include "LogLevels.h"

#include <optional>
#include <string>

/**
 * @brief Basic configuration that every target contains
 *
 */
struct SagradaConfig {
	std::optional<std::string> logFileName;
	log_level                  minLogLevel;
};

#endif // SAGRADA_CONFIG_H