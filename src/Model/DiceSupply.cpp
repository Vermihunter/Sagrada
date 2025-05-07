#include "DiceSupply.h"

/** Dice initialization method at the beginning of the game
 *
 * Deletes previous set of dice and generates a new one
 */
DiceSupply::DiceSupply(size_t _dicePerRound, dice_c _dice, RandomGenerator& _rnd)
    : dicePerRound(_dicePerRound), dice(std::move(_dice)), rnd(_rnd)
{
}

DiceSupply::DiceSupply(const DiceSupply& other, RandomGenerator& _rnd)
    : dicePerRound(other.dicePerRound), rnd(_rnd)
{
	dice = std::make_unique<std::vector<Die>>(*other.dice);
}

int DiceSupply::get_index_of_die(Die* d) const
{
	ptrdiff_t index = d - &((*dice)[0]);
	if (index < 0 || index >= int(dice->size())) {
		return -1;
	}
	return int(index);
}

Die* DiceSupply::get_die_with_index(int ind) const
{
	if (ind < 0 || size_t(ind) >= dice->size()) {
		return nullptr;
	}

	return &((*dice)[ind]);
}

/**
 * Returns the appropriate dice from the set of available dice
 *
 * Since the dice contain only small amount of data and there is
 * not much of them, we can afford copying them.
 *
 * @return Vector of dice that could be used in the next round
 */
std::vector<Die*> DiceSupply::get_current_round_dice(size_t currentRound)
{
	/** There are not enough dice in the supply for a next round */
	if ((currentRound + 1) * dicePerRound > dice->size()) {
		throw std::invalid_argument{"There are not enough dice in the supply for round " +
		                            std::to_string(currentRound)};
	}

	std::vector<Die*> next_round_dice(dicePerRound);
	for (size_t i = 0; i < dicePerRound; ++i) {
		next_round_dice[i] = &(dice->at(i + dicePerRound * currentRound));
	}

	return next_round_dice;
}

Die* DiceSupply::get_unused_die(size_t currentRound)
{
	int lastUsedDieInd           = int((currentRound)*dicePerRound);
	int currentlyUnusedDiceCount = int(dice->size()) - lastUsedDieInd;

	if (currentlyUnusedDiceCount <= 0) {
		return nullptr;
	}

	auto randomIndexFromUnused = rnd.random_in_interval(0, currentlyUnusedDiceCount - 1);
	return &((*dice)[lastUsedDieInd + randomIndexFromUnused]);
}

void DiceSupply::randomize_future_dice(size_t currentRound)
{
	size_t firstNewIndex = (currentRound * dicePerRound);
	rnd.shuffle(dice->begin() + firstNewIndex, dice->end());

	for (size_t i = firstNewIndex; i < dice->size(); ++i) {
		(*dice)[i].change_value(rnd.roll_die());
	}
}

bool DiceSupply::empty(size_t currentRound)
{
	int lastUsedDieInd           = int((currentRound)*dicePerRound);
	int currentlyUnusedDiceCount = int(dice->size()) - lastUsedDieInd;

	return currentlyUnusedDiceCount <= 0;
}

Die* get_corresponding_die(Die* die, const DiceSupply& from, const DiceSupply& to)
{
	int  ind    = from.get_index_of_die(die);
	Die* newDie = to.get_die_with_index(ind);
	if (newDie == nullptr || !(*die == *newDie)) {
		throw std::invalid_argument{"Invalid die transformation"};
	}
	return newDie;
}
