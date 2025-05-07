#ifndef BOARD_FIELD_GENERAL_PLACEABLE_DICE_H
#define BOARD_FIELD_GENERAL_PLACEABLE_DICE_H

#include "Attributes.h"
#include "Die.h"

#include <array>
#include <exception>

constexpr size_t val_size = size_t(value_t::COUNT);
constexpr size_t col_size = size_t(color_t::COUNT);

struct AbsoluteIndexMapper {
	static size_t get_abs_col_ind(size_t i, size_t relColInd) { return (relColInd * col_size) + i; }

	static size_t get_abs_val_ind(size_t i, size_t relValInd) { return (i * col_size) + relValInd; }
};

struct GeneralPlaceableDice {
	/**
	 * @brief Flattened 2D array, first 'val_size' elements represent all values
	 * having color color_t(0), color_t(1)...
	 */
	std::array<bool, val_size * col_size> placeableDice;

	/**
	 * starting from Least-significant bit (from now LSb), each 6 bits represent
	 * the 6 value of one color
	 */
	all_dice_t placeableDice;

	FastPlaceableDice() { reset(); }

	void reset() { placeableDice.fill(true); }

	bool any_having_color(color_t color)
	{
		return any_having_attribute(color, color_size, &AbsoluteIndexMapper::get_abs_col_ind);
	}

	bool any_having_value(value_t value)
	{
		return any_having_attribute(value, val_size, &AbsoluteIndexMapper::get_abs_val_ind);
	}

	void allow(color_t color, value_t value)
	{
		allow_color(color);
		allow_value(value);
	}

	void allow_color(color_t color) { throw std::invalid_argument{"Not implemented"}; }

	void allow_value(value_t value) { throw std::invalid_argument{"Not implemented"}; }

	void disallow(Die* die)
	{
		if (!die) return;

		disallow_color(die->get_color());
		disallow_value(die->get_value());
	}

	void disallow_value(value_t value)
	{
		disallow_attribute(value, val_size, &AbsoluteIndexMapper::get_abs_val_ind);
	}

	void disallow_color(color_t color)
	{
		disallow_attribute(color, col_size, &AbsoluteIndexMapper::get_abs_col_ind);
	}

	bool placeable_concrete(color_t color, value_t value)
	{
		size_t valueInd = size_t(value);
		size_t colorInd = size_t(color);

		if (colorInd >= col_size || valueInd >= val_size) {
			return false;
		}

		return placeableDice[colorInd * col_size + valueInd];
	}

	/** Debug */
	void print_all_placeable()
	{
		std::cout << std::boolalpha;
		for (size_t i = 0; i < Color::SIZE; ++i) {
			for (size_t j = 0; j < Value::SIZE; ++j) {
				color_t c = color_t(i);
				value_t v = value_t(j);

				std::cout << "[" << Color::type_to_string(c) << ", " << j + 1
				          << "] : " << placeable_concrete(c, v) << std::endl;
			}
		}
	}

	template <typename Attribute>
	bool any_having_attribute(Attribute attr, const size_t attrSize,
	                          size_t (*indMapper)(size_t, size_t))
	{
		size_t relAttrInd = size_t(attr);

		if (relAttrInd >= attrSize) {
			return false;
		}

		for (size_t i = 0; i < col_size; ++i) {
			size_t absAttrInd = indMapper(i, relAttrInd);
			if (placeableDice[absAttrInd]) {
				return true;
			}
		}

		return false;
	}

	template <typename Attribute>
	void disallow_attribute(Attribute attr, const attrSize, size_t (*indMapper)(size_t, size_t))
	{
		size_t relAttrInd = size_t(attr);

		if (relAttrInd >= attrSize) return;

		for (size_t i = 0; i < col_size; ++i) {
			size_t absAttrInd         = indMapper(i, relAttrInd);
			placeableDice[absAttrInd] = false;
		}
	}
};

#endif // BOARD_FIELD_GENERAL_PLACEABLE_DICE_H