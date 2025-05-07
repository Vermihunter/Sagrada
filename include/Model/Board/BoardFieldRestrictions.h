#ifndef BOARD_FIELD_RESTRICTIONS_H
#define BOARD_FIELD_RESTRICTIONS_H

#include "Attributes.h"

struct BoardFieldRestrictions {
	std::optional<value_t> valueRestriction;
	std::optional<color_t> colorRestriction;

	auto clone() { return std::make_unique<BoardFieldRestrictions>(*this); }
};

#endif // BOARD_FIELD_RESTRICTIONS_H