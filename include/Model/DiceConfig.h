#ifndef DICE_CONFIG_H
#define DICE_CONFIG_H

#include <memory>
#include <unordered_map>

#include "Attributes.h"

/**
 * @brief A wrapper around a given colored dice
 *
 */
struct OneColoredDiceConfig
{
    OneColoredDiceConfig() : diceCount(0) {}

    size_t diceCount;
};

/**
 * @brief Represents the dice configuration that will be used in the GameContext
 * construction
 */
struct DiceConfig
{
    DiceConfig()
    {
        for (size_t i = 0; i < size_t(color_t::COUNT); ++i) {
            diceByColor.insert({color_t(i), OneColoredDiceConfig {}});
        }
    }

    std::unordered_map<color_t, OneColoredDiceConfig> diceByColor;
};

using dice_config_c = std::unique_ptr<DiceConfig>;

#endif // DICE_CONFIG_H