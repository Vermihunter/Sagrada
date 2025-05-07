#ifndef WPC_TEXT_FILE_READER_H
#define WPC_TEXT_FILE_READER_H

#include "BoardReader.h"
#include "BoardType.h"

#include <iostream>
#include <iterator>

/**
 * @brief Reader class for the defined rectangular format from text file
 *
 */
struct BoardTextFileReader : BoardReader {
	BoardTextFileReader(std::ifstream& _sourceFileStream) : sourceFileStream(_sourceFileStream) {}
	virtual ~BoardTextFileReader() = default;

	virtual void append_wpc(wpc_cache_t& dest) override;

  private:
	std::ifstream& sourceFileStream;

	enum class READ_STATE {
		WINDOW_NAME,
		WINDOW_HARDNESS,
		WINDOW_CONFIG,
		WINDOW_DATA,
		CARD_DIVIDER
	} state;

	inline const static std::string cardDivider = "-----";

	/** Debug -> better help message to recover incorrect file */
	unsigned int linesRead;

	board_ctx_t                            currCtx;
	std::unique_ptr<BoardDataInputHandler> currBoardInputHandler;

	bool construct_with_title(std::string title);
	bool process_line_divider(const std::string& line, wpc_cache_t& dest);
	bool process_data_row(std::string dataRow);
	bool construct_config(const std::string& dimensions);
	bool get_board_type(const std::string& line);
	bool add_hardness(const std::string& hardnessStr);
};

/**
 * @brief Helper functions for the WPC parsing
 * All of them return true indicating whether the sub-parsing was correct or
 * not.
 */
bool to_uint(const std::string& str, unsigned int& dest);

#endif // WPC_TEXT_FILE_READER_H