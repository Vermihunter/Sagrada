#include "Attributes.h"

#include "StringAttributes.h"

#include <iostream>

Color red{.r = 255.0 / 255.0, .g = 015.0 / 255.0, .b = 008.0 / 255.0, .a = 1.0};
Color green{.r = 117.0 / 255.0, .g = 255.0 / 255.0, .b = 077.0 / 255.0, .a = 1.0};
Color blue{.r = 125.0 / 255.0, .g = 224.0 / 255.0, .b = 255.0 / 255.0, .a = 1.0};
Color yellow{.r = 245.0 / 255.0, .g = 255.0 / 255.0, .b = 028.0 / 255.0, .a = 1.0};
Color purple{.r = 158.0 / 255.0, .g = 099.0 / 255.0, .b = 252.0 / 255.0, .a = 1.0};
Color white{.r = 255.0 / 255.0, .g = 255.0 / 255.0, .b = 255.0 / 255.0, .a = 1.0};
Color black{.r = 000.0 / 255.0, .g = 000.0 / 255.0, .b = 000.0 / 255.0, .a = 1.0};
Color grey{.r = 105.0 / 255.0, .g = 105.0 / 255.0, .b = 105.0 / 255.0, .a = 1.0};
Color orange{.r = 255.0 / 255.0, .g = 140.0 / 255.0, .b = 0.0 / 255.0, .a = 1.0};

Color& Color::type_to_rgba(std::optional<color_t> c)
{
	if (!c.has_value()) {
		return white;
	}

	switch (c.value()) {
	case color_t::RED:
		return red;
		break;
	case color_t::GREEN:
		return green;
		break;
	case color_t::BLUE:
		return blue;
		break;
	case color_t::YELLOW:
		return yellow;
		break;
	case color_t::PURPLE:
		return purple;
		break;
	case color_t::COUNT:

		break;
	default:
		break;
	}

	return white;
}

bool Value::val_is_correct(int val)
{
	if (val < 0 || val >= int(Value::SIZE)) {
		return false;
	}

	return true;
}

/** debug */
std::string Color::type_to_string(std::optional<TYPE> c)
{
	if (!c.has_value()) {
		return "EMPTY COLOR";
	}

	return color_to_string.at(c.value());
}
