#ifndef DIE_JSON_H
#define DIE_JSON_H

#include "Die.h"
#include "SagradaJson.h"
#include "StringAttributes.h"

void to_json(json& j, const Die* die)
{
	if (!die) {
		j = json{};
	}
	else {
		j = json{{"value", value_to_string.at(die->get_value())},
		         {"color", color_to_string.at(die->get_color())}};
	}
}

#endif // DIE_JSON_H