#ifndef PUBLIC_OBJECTIVE_CARD_TYPE_H
#define PUBLIC_OBJECTIVE_CARD_TYPE_H

/**
 * @brief Defines all types of Public Objective Cards used in game
 *
 * The order of the types doesn't have any effect.
 * Only the types that are defined BEFORE 'COUNT' element
 * are included in the game. The others have 0% of chance being picked.
 */
enum class puoc_etype
{
    UNIQUE_VALUE_ROW,
    UNIQUE_COLOR_ROW,
    UNIQUE_VALUE_COLUMN,
    UNIQUE_COLOR_COLUMN,
    ONE_AND_TWO_COLLECTING,
    THREE_AND_FOUR_COLLECTING,
    FIVE_AND_SIX_COLLECTING,
    ALL_VALUE_COLLECTING,
    ALL_COLOR_COLLECTING,
    DIAGONAL_CONNECTING,
    COUNT
};

#endif // PUBLIC_OBJECTIVE_CARD_TYPE_H