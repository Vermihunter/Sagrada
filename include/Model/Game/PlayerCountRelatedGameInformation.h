#ifndef PLAYER_COUNT_RELATED_GAME_INFORMATION_H
#define PLAYER_COUNT_RELATED_GAME_INFORMATION_H

/**
 * @brief Represents information used for GameContextBuilder that is related to
 * player count
 *
 * According to the official rules of Sagrada there exists a function that would
 * return the diceCount for each player count ( diceCount = (2 * playerCount) +
 * 1) but this class is defined to make the system more customizable
 */
struct PlayerCountRelatedGameInformation
{
    size_t playerCount;
    size_t diceCount;
};

#endif // PLAYER_COUNT_RELATED_GAME_INFORMATION_H