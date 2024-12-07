#ifndef ROUND_TRACK_H
#define ROUND_TRACK_H

#include <array>
#include <memory>
#include <stack>
#include <vector>

#include "Constants.h"
#include "DiceSupply.h"
#include "Die.h"

/**
 * @brief Stores dice which were unused or replaced in any of the previous
 * rounds
 *
 */
class RoundTrack
{
    std::vector<Die*> _dice;
    std::stack<size_t> diceCountPerRound;

  public:
    RoundTrack() {}

    ~RoundTrack() {}

    RoundTrack(const RoundTrack& obj, DiceSupply& newDS,
               DiceSupply& originalDS);

    auto clone(DiceSupply& newDS, DiceSupply& originalDS)
    {
        return std::make_unique<RoundTrack>(*this, newDS, originalDS);
    }

    /**
     * Returns dice used in the last round
     */
    std::vector<Die*> undo();

    size_t dice_count() { return _dice.size(); }

    /**
     * @brief Returns the dice that are placed to the round track
     *
     * @return auto& vector of dice
     */
    auto& get_dice() const { return _dice; }

    /**
     * @brief Puts the dice after the last move of the round to the round track
     *
     * @param dice the dice to put
     */
    void put_dice(std::vector<Die*>& dice);

    /**
     * @brief Replaces a die with another
     * Used by ReplaceDieFromSupplyToRoundTrackTC
     *
     * @param dieToReplace
     * @param replaceWith
     */
    void replace_die(Die* dieToReplace, Die* replaceWith);

    /**
     * @brief Checks whether is there a die containing a given color
     * Used when checkin correctness of a
     * ReplaceDieFromSupplyToRoundTrackTC::Move.
     *
     * @param col color that is looked for
     * @return true if there is a die with given color
     * @return false otherwise
     */
    bool contains_die_with_color(color_t col);
};

#endif
