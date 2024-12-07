#include "BoardTextFileReader.h"

#include "RectangularBoardType.h"

void BoardTextFileReader::append_wpc(wpc_cache_t& dest)
{
    if (!sourceFileStream.good()) {
        LOG_E("Wrong Window pattern card input stream!");
        return;
    }

    // Initialize the RectangularBoardType with its static initializer
    RectangularBoardType::get();

    /** Debug->better help message to recover incorrect file */
    linesRead = 0;
    state = READ_STATE::WINDOW_NAME;
    currCtx = nullptr;

    std::string line;
    bool prevLineCorrect = true;

    while (std::getline(sourceFileStream, line)) {
        /**
         * @brief There was an error in the Window pattern card representation
         * in the txt file, so the next rows are ignored until the next card
         * divider is reached. This may help both debugging (more error messages
         * are displayed in a single run) and running with the remaining,
         * correctly represented Board.
         */
        if (!prevLineCorrect && line != cardDivider) {
            continue;
        }

        switch (state) {
            case READ_STATE::WINDOW_NAME:
                prevLineCorrect = construct_with_title(std::move(line));
                break;

            case READ_STATE::WINDOW_HARDNESS:
                prevLineCorrect = add_hardness(line);
                break;

            case READ_STATE::WINDOW_CONFIG:
                prevLineCorrect = construct_config(line);
                break;

            case READ_STATE::WINDOW_DATA:
                prevLineCorrect = process_data_row(std::move(line));
                break;

            case READ_STATE::CARD_DIVIDER:
                prevLineCorrect = process_line_divider(line, dest);
                break;

            default:
                break;
        }

        ++linesRead;
    }
}

bool BoardTextFileReader::construct_with_title(std::string title)
{
    currCtx = std::make_unique<BoardContext>();
    currCtx->title = std::move(title);

    state = READ_STATE::WINDOW_HARDNESS;
    return true;
}

bool BoardTextFileReader::add_hardness(const std::string& hardnessStr)
{
    bool uintConversionSuccessful = to_uint(hardnessStr, currCtx->hardness);

    if (!uintConversionSuccessful)
        return false;

    state = READ_STATE::WINDOW_CONFIG;
    return true;
}

bool to_uint(const std::string& str, unsigned int& dest)
{
    try {
        std::size_t pos;
        dest = std::stoul(str, &pos);

        if (pos != str.size()) {
            throw std::invalid_argument {
                "\"" + str + "\" is not a correct unsigned intiger!"};
        }
    }
    catch (std::exception& e) {
        return false;
    }

    return true;
}

bool BoardTextFileReader::construct_config(const std::string& wpcConfig)
{
    const std::string errorMsg = "Invalid Config on line " +
                                 std::to_string(linesRead) +
                                 "! Config must have format UINT UINT TYPE";

    std::stringstream ss(wpcConfig);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> configParts(begin, end);

    if (configParts.size() != 3) {
        LOG_E(errorMsg);
        return false;
    }

    unsigned int boardWidth;
    unsigned int boardHeight;

    if (!to_uint(configParts[0], boardWidth) ||
        !to_uint(configParts[1], boardHeight)) {
        LOG_E(errorMsg);
        return false;
    }

    BoardType* type = BoardType::get_type(configParts[2]);

    if (type == nullptr) {
        return false;
    }

    currCtx->config = BoardConfig::get_config(boardWidth, boardHeight, type);
    currBoardInputHandler =
        type->construct_input_handler(currCtx->data, currCtx->config);

    state = READ_STATE::WINDOW_DATA;
    return true;
}

bool BoardTextFileReader::process_data_row(std::string dataRow)
{
    BoardRowParsingFeedBack feedBack =
        currBoardInputHandler->handle_line(dataRow);

    if (feedBack.lastLine) {
        state = READ_STATE::CARD_DIVIDER;
    }

    return feedBack.lineCorrect;
}

bool BoardTextFileReader::process_line_divider(const std::string& line,
                                               wpc_cache_t& dest)
{
    if (line != cardDivider) {
        LOG_E("Card divider was incorrect - line " + std::to_string(linesRead));
        return false;
    }

    auto BoardContainerIt = dest.find(currCtx->config);
    if (BoardContainerIt != dest.end()) {
        BoardContainerIt->second.push_back(std::move(currCtx));
    }
    else {
        dest.insert({currCtx->config, wpc_c {std::move(currCtx)}});
    }

    currBoardInputHandler = nullptr;
    currCtx = nullptr;
    state = READ_STATE::WINDOW_NAME;
    return true;
}