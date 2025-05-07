#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

#include "BoardType.h"

#include <algorithm>
#include <list>
#include <memory>

struct BoardReader;

/**
 * @brief Represents a configuration for a given board
 *
 * The constructor is private and defines a friend class WPCReader
 * because this way only the WPCReader can construct new objects.
 * This is important because the user should not construct these objects
 * without assigning it to a concrete board.
 *
 */
struct BoardConfig {
	const size_t     width;
	const size_t     height;
	const BoardType* type;

	/**
	 * @brief Returns the config with the given parameters
	 * It uses a cache so if the config already existed, it returns the
	 * instance. Otherwise, it creates a new config with the provided parameters
	 * and returns the new one.
	 *
	 * @param _width Width of the board
	 * @param _height Height of the board
	 * @param _type Type of the board
	 * @return BoardConfig* Cached config of the board
	 */
	static BoardConfig* get_config(size_t _width, size_t _height, BoardType* _type);

  private:
	BoardConfig(size_t _width, size_t _height, BoardType* _type)
	    : width(_width), height(_height), type(_type)
	{
	}
};

#endif // BOARD_CONFIG_H