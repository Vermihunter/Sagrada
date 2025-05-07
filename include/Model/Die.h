#ifndef DIE_H
#define DIE_H

#include "Attributes.h"

/**
 * @brief Represents a die in the game.
 */
class Die {
	/** Attributes */
	color_t c;
	value_t v;

  public:
	/**
	 * Each die must contain both color and value and for that reason,
	 * there exists only constructor that takes both of them.
	 *
	 * @param c Color of the die
	 * @param v Value of the die
	 */
	Die(color_t _c, value_t _v) : c(_c), v(_v) {}

	/** Getters */
	auto get_value() const { return v; }
	auto get_color() const { return c; }

	/** Setters - used by Tool card moves */
	void change_value(value_t newVal) { v = newVal; }
	void change_color(color_t newCol) { c = newCol; }

	/** For generic processing - e.g. Public objective card pattern searches */
	template <typename A> constexpr static auto get_attr(Die* die)
	{
		if constexpr (std::is_same_v<A, value_t>) return die->get_value();
		if constexpr (std::is_same_v<A, color_t>) return die->get_color();
	}

	bool operator==(const Die& other) { return v == other.v && c == other.c; }

	/** Debug */
	friend std::ostream& operator<<(std::ostream& os, const Die* die)
	{
		return os << "Die: [" << Color::type_to_string(die->c) << " val: " << size_t(die->v) + 1
		          << "]";
	}
};

#endif // DIE_H
