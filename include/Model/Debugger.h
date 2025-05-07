#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "Logger.h"
#include "Move.h"
#include "Typedefs.h"

inline void print_all_moves(const move_c& moves, const std::string& msg)
{
	LOG_T(msg);
	LOG_T("#######################");
	for (auto&& move : moves) {
		LOG_T(move);
	}
	LOG_T("#######################");
}

#endif // DEBUGGER_H