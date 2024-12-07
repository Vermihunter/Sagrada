#ifndef DIE_TO_FIELD_MOVE_MODE_H
#define DIE_TO_FIELD_MOVE_MODE_H

#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>

using dtfm_mode_mask_t = std::uint16_t;

/**
 * @brief Defines the modes that a die to field move can have
 * Defines values so it could be used as a bitmask to allow using multiple
 * modes at the same time.
 *
 * The functionality of combining multiple modes is currently unused.
 */
enum dtfm_mode_t : dtfm_mode_mask_t
{
    DTFM_MODE_DEFAULT = 0,
    DTFM_MODE_IGNORE_VALUE = 1,
    DTFM_MODE_IGNORE_COLOR = 2,
    DTFM_MODE_ACCEPT_UNREACHABLE = 4
};

/* Debugging mode-to-string mapper */
const std::unordered_map<dtfm_mode_t, std::string> dtfmModeToStr {
    {DTFM_MODE_DEFAULT, "default"},
    {DTFM_MODE_IGNORE_VALUE, "ignore color"},
    {DTFM_MODE_IGNORE_COLOR, "ignore value"},
    {DTFM_MODE_ACCEPT_UNREACHABLE, "accept unreachable"}};

/* Debugging functions */
inline std::ostream& print_mode(std::ostream& os, dtfm_mode_mask_t modeMask)
{
    os << "mode: [";
    for (auto&& [mode, modeStr] : dtfmModeToStr) {
        if ((modeMask & mode) > 0) {
            os << modeStr << ",";
        }
    }

    return os << "]";
}

#endif // DIE_TO_FIELD_MOVE_MODE_H