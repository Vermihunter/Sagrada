#include "RoundTrack.h"

RoundTrack::RoundTrack(const RoundTrack& obj, DiceSupply& newDS,
                       DiceSupply& originalDS)
    : diceCountPerRound(obj.diceCountPerRound)
{
    for (auto&& die : obj._dice) {
        int ind = originalDS.get_index_of_die(die);
        _dice.push_back(newDS.get_die_with_index(ind));
    }
}

void RoundTrack::replace_die(Die* replaceWith, Die* dieToReplace)
{
    auto dieIt = std::find(_dice.begin(), _dice.end(), dieToReplace);
    if (dieIt != _dice.end()) {
        std::swap(*replaceWith, *dieToReplace);
    }
}

bool RoundTrack::contains_die_with_color(color_t col)
{
    for (auto&& die : _dice) {
        if (die->get_color() == col) {
            return true;
        }
    }

    return false;
}

void RoundTrack::put_dice(std::vector<Die*>& diceToPut)
{
    _dice.insert(_dice.end(), diceToPut.begin(), diceToPut.end());
    diceCountPerRound.push(diceToPut.size());
}

std::vector<Die*> RoundTrack::undo()
{
    std::vector<Die*> dice;

    if (diceCountPerRound.empty()) {
        // ERROR
        return dice;
    }

    size_t lastRoundDiceCount = diceCountPerRound.top();
    diceCountPerRound.pop();

    for (size_t i = _dice.size() - lastRoundDiceCount; i < _dice.size(); ++i) {
        dice.push_back(_dice[i]);
    }

    _dice.resize(_dice.size() - lastRoundDiceCount);
    return dice;
}
