#ifndef MINIMAX_PLAYER_TYPE_H
#define MINIMAX_PLAYER_TYPE_H

#include <cmath>

/**
 * @brief Templated class for representing the minimum and maximum values for
 * the chosen heuristic value type
 *
 * @tparam T
 */
template <typename T> struct MinMaxVal {
};

/**
 * @brief The current implementation uses `int` heuristic value types
 *
 * @tparam
 */
template <> struct MinMaxVal<int> {
	static constexpr int min = INT_MIN;
	static constexpr int max = INT_MAX;
};

/**
 * @brief Base class for the Minimizing and Maximizing players for the Minimax
 * algorithm
 *
 * @tparam T
 */
template <typename T> struct MinimaxPlayerType {
	MinimaxPlayerType* otherPlayer;

	virtual ~MinimaxPlayerType() = default;

	virtual T    get_initial_value()                           = 0;
	virtual bool is_better_value(T currVal, T currBest)        = 0;
	virtual void update_alpha_beta(T& alpha, T& beta, T value) = 0;
};

template <typename T> struct MinimizingPlayer : MinimaxPlayerType<T> {
	virtual T get_initial_value() override { return MinMaxVal<T>::max; }

	virtual bool is_better_value(T currVal, T currBest) override { return currVal < currBest; }

	virtual void update_alpha_beta(T& alpha, T& beta, T value) override
	{
		beta = std::min(beta, value);
	}
};

template <typename T> struct MaximizingPlayer : MinimaxPlayerType<T> {
	virtual T get_initial_value() override { return MinMaxVal<T>::min; }

	virtual bool is_better_value(T currVal, T currBest) override { return currVal > currBest; }

	virtual void update_alpha_beta(T& alpha, T& beta, T value) override
	{
		alpha = std::max(alpha, value);
	}
};

#endif // MINIMAX_PLAYER_TYPE_H