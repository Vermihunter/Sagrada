#include "BoardConfig.h"

BoardConfig* BoardConfig::get_config(size_t _width, size_t _height,
                                     BoardType* _type)
{
    static std::list<BoardConfig> allBoardConfigs;

    const auto configByParameters = [&](const BoardConfig& config) {
        return config.width == _width && config.height == _height &&
               config.type == _type;
    };

    auto boardConfigIt = std::find_if(
        allBoardConfigs.begin(), allBoardConfigs.end(), configByParameters);

    BoardConfig* res;
    if (boardConfigIt == allBoardConfigs.end()) {
        allBoardConfigs.push_back(BoardConfig {_width, _height, _type});
        res = &allBoardConfigs.back();
    }
    else {
        res = &(*boardConfigIt);
    }

    return res;
}