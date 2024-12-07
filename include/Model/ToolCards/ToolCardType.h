#ifndef TOOL_CARD_TYPE_H
#define TOOL_CARD_TYPE_H

/**
 * @brief The enumerated representation of all Tool card types
 */
enum class tc_etype
{
    CHANGE_DIE_VALUE,
    MOVE_TWICE,
    DIE_WITHOUT_NEIGHBORS,
    REROLL_ALL_DICE,
    RELOCATE_TWO_SAME_COLORED_DICE,
    REPLACE_FROM_ROUND_TRACK,
    RANDOM_BAG_DIE,
    REROLL_DIE,
    REVERSE_DIE_VALUE,
    MOVE_IGNORE_VAL,
    MOVE_IGNORE_COLOR,
    RELOCATE_TWO_DICE,
    COUNT
};

#endif // TOOL_CARD_TYPE_H