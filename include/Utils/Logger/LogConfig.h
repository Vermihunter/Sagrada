#ifndef LOG_CONFIG_H
#define LOG_CONFIG_H

#include "LogLevels.h"

#include <iostream>

struct DefaultLogSettings {
	constexpr static log_level  minLogLevel = log_level::INFO;
	inline static std::ostream* logStream   = &std::cout;

	inline static const std::string logDir = "log";
};

struct LogConfig {
	LogConfig()
	{
		minLogLevel = DefaultLogSettings::minLogLevel;
		logStream   = DefaultLogSettings::logStream;
	}

	log_level     minLogLevel;
	std::ostream* logStream;
};

#endif