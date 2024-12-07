#ifndef DEFAULT_GAME_CONSTANTS_H
#define DEFAULT_GAME_CONSTANTS_H

#include <stddef.h>

#include <string>

/** Public objective cards */
constexpr int defaultPuocPerGame = 3;

/** Tool cards */
constexpr int defaultTcPerGame = 3;

/** Dice */
constexpr int defaultDicePerColor = 18;
constexpr int defaultDicePerPlayer = 2;
constexpr int defaultExtraDice = 1;

constexpr int defaultNumberOfRounds = 10;
constexpr int defaultWPCChoicePerPlayer = 4;

constexpr size_t defaultMaxPlayers = 4;

constexpr size_t defaultBoardWidth = 5;
constexpr size_t defaultBoardHeight = 4;
const std::string defaultBoardType = "Rectangle";

#endif // DEFAULT_GAME_CONSTANTS_H