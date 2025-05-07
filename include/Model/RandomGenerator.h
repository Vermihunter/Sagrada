#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H

#include "Attributes.h"
#include "Logger.h"
#include "Typedefs.h"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>

/**
 * @brief Generates randomness
 * Used during initialization and when the choice has to be random
 */
class RandomGenerator {
	std::mt19937 eng;

  public:
	RandomGenerator() = default;

	static auto initialize_with_curr_time()
	{
		auto gen = std::make_unique<RandomGenerator>();
		gen->eng.seed(time(nullptr));
		return gen;
	}

	static auto initialize_with_seed(int seed)
	{
		auto gen = std::make_unique<RandomGenerator>();
		gen->eng.seed(seed);
		return gen;
	}

	auto clone() { return std::make_unique<RandomGenerator>(*this); }

	/** Since rolling a die is very common, the distribution is cached */
	value_t roll_die() { return value_t(random_in_interval(0, size_t(value_t::COUNT) - 1)); }

	/** Returns a value in interval [from, to] */
	unsigned int random_in_interval(unsigned int from, unsigned int to)
	{
		return eng() % (to - from + 1) + from;
	}

	/**
	 * @brief Shuffles a container
	 * Used when initializing different types of random
	 * cards at the beginning of a game.
	 *
	 * @tparam C reference to the container
	 */
	template <typename Container> void shuffle(Container& container)
	{
		// std::shuffle( container.begin(), container.end(), eng );
		shuffle(container.begin(), container.end());
	}

	template <typename _It> void shuffle(_It first, _It last) { std::shuffle(first, last, eng); }

	std::vector<ID_t> generate_random_id_order(unsigned int maxValue)
	{
		std::vector<ID_t> idOrder(maxValue);
		std::iota(idOrder.begin(), idOrder.end(), 0);

		shuffle(idOrder);
		return idOrder;
	}
};

using rnd_t = std::unique_ptr<RandomGenerator>;

#endif
