#ifndef MOVE_JSON_H
#define MOVE_JSON_H

#include "Move.h"
#include "SagradaJson.h"

#include <sstream>

void to_json(json& j, const move_t& move)
{
	if (!move) {
		j = json{};
		return;
	}

	std::ostringstream oss;
	oss << move;
	j = json{{"", oss.str()}};
}

#endif // MOVE_JSON_H