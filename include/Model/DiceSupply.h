#ifndef DICE_SUPPLY_H
#define DICE_SUPPLY_H

#include "Constants.h"
#include "Die.h"
#include "RandomGenerator.h"

#include <algorithm>
#include <array>
#include <memory>
#include <vector>

using dice_c = std::unique_ptr<std::vector<Die>>;

/**
 * @brief Represents the collection of dice in a given game
 * The die pointers used across the project point to dice stored in DiceSupply
 * objects.
 */
class DiceSupply {
	size_t dicePerRound;
	/** Stores the dice for the whole game. */
	dice_c dice;
	/** Used in the get_unused_die() so it returned a random unused die. */
	RandomGenerator& rnd;

  public:
	DiceSupply(size_t dicePerRound, dice_c _dice, RandomGenerator& rnd);

	/**
	 * @brief Construct a new Dice Supply object for cloning
	 *
	 * @param ds The DiceSupply to be cloned
	 * @param rnd The random generator that is used
	 */
	DiceSupply(const DiceSupply& ds, RandomGenerator& rnd);

	auto clone(RandomGenerator& rnd) { return std::make_unique<DiceSupply>(*this, rnd); }

	/**
	 * @brief These two functions help to convert a die from one DiceSupply
	 * objet to another It is highly used during the cloning procedures
	 */
	int  get_index_of_die(Die* d) const;
	Die* get_die_with_index(int ind) const;

	// Will be maybe unused
	void randomize_future_dice(size_t currentRound);

	/**
	 * @brief Checks if there are any dice in the bag remaining.
	 * Used by some tool cards (e.g. ReplaceDieWithRandomFromBagTC)
	 *
	 * @return true if there are no dice in the bag
	 * @return false if there is at least one die in the bag
	 */
	bool empty(size_t currentRound);

	/**
	 * @brief Returns the dice available in the current round
	 *
	 * @return std::vector<Die*>  list of pointers to the dice
	 */
	std::vector<Die*> get_current_round_dice(size_t currentRound);

	/**
	 * @brief Returns a die that is still in the bag
	 */
	Die*                    get_unused_die(size_t currentRound);
	const std::vector<Die>& get_all_dice() const { return *dice; }
};

Die* get_corresponding_die(Die* die, const DiceSupply& from, const DiceSupply& to);

#endif // DICE_SUPPLY_H
