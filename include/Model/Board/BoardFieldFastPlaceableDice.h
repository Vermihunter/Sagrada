#ifndef BOARD_FIELD_FAST_PLACEABLE_DICE_H
#define BOARD_FIELD_FAST_PLACEABLE_DICE_H

#include "Attributes.h"
#include "Die.h"

#include <exception>

using all_dice_t = std::uint32_t;

/**
 * @brief Values representing all the attributes of a given kind
 * The color_bit_mask variable has the last Value::SIZE LS bits set to 1, the
 * others to 0, the value_one_bit_mask is set in a way, that for each color it
 * has 1 in the position of value 1, the others are 0.
 *
 * This implementation of these masks makes easier to use them, because shifting
 * by N, gets if a given attribute is present or not e.g. shifting the
 * value_one_bit_mask by 3 and using it with a & operator has a non-zero value
 * if any die having value 3 could be put on a field.
 */
constexpr all_dice_t color_bit_mask     = ~(all_dice_t(0)) >> (32 - Value::SIZE);
constexpr all_dice_t value_one_bit_mask = 0b00000001000001000001000001000001;

template <typename A> struct BitmaskGenerator {
};

template <> struct BitmaskGenerator<Color> {
	static all_dice_t get_have_bitmask(size_t colorInd)
	{
		return (color_bit_mask << (colorInd * Color::SIZE));
	}

	static all_dice_t get_disallow_bitmask(size_t colorInd)
	{
		all_dice_t thisColorMask = color_bit_mask << (Value::SIZE * colorInd);
		all_dice_t bitmask       = ~(all_dice_t(0)) ^ thisColorMask;
		return bitmask;
	}
};

template <> struct BitmaskGenerator<Value> {
	static all_dice_t get_have_bitmask(size_t valueInd) { return (value_one_bit_mask << valueInd); }

	static all_dice_t get_disallow_bitmask(size_t valueInd)
	{
		all_dice_t thisValueMask = (value_one_bit_mask << valueInd);
		all_dice_t bitmask       = ~(all_dice_t(0)) ^ thisValueMask;
		return bitmask;
	}
};

struct FastPlaceableDice {
	/**
	 * starting from Least-significant bit (from now LSb), each 6 bits represent
	 * the 6 value of one color
	 */
	all_dice_t placeableDice;

	FastPlaceableDice() { reset(); }

	void reset()
	{
		// The first two MSb are set to 0, the others are set to 1
		placeableDice = (~(all_dice_t(0))) >> 2;
	}

	bool any_having_color(color_t color) { return any_having_attribute<Color>(color); }

	bool any_having_value(value_t value) { return any_having_attribute<Value>(value); }

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

	void disallow_value(value_t value) { disallow_attribute<Value>(value); }

	void disallow_color(color_t color) { disallow_attribute<Color>(color); }

	bool placeable_concrete(color_t color, value_t value)
	{
		size_t valueInd = size_t(value);
		size_t colorInd = size_t(color);

		if (colorInd >= Color::SIZE || valueInd >= Value::SIZE) {
			return false;
		}

		all_dice_t valueMask    = color_bit_mask & (1 << valueInd);
		all_dice_t concreteMask = (valueMask << (Value::SIZE * colorInd));

		return (placeableDice & concreteMask) > 0;
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

	template <typename A> bool any_having_attribute(A::TYPE attr)
	{
		size_t attrInd = size_t(attr);

		if (attrInd >= A::SIZE) return false;

		all_dice_t bitmask = BitmaskGenerator<A>::get_have_bitmask(attrInd);
		return (placeableDice & bitmask) > 0;
	}

	template <typename A> void disallow_attribute(A::TYPE attr)
	{
		size_t attrInd = size_t(attr);

		if (attrInd >= A::SIZE) return;

		all_dice_t bitmask = BitmaskGenerator<A>::get_disallow_bitmask(attrInd);
		placeableDice &= bitmask;
	}
};

#endif // BOARD_FIELD_FAST_PLACEABLE_DICE_H
