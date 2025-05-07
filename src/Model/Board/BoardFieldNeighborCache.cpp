#include "BoardFieldNeighborCache.h"

const std::vector<std::pair<int, int>> BoardFieldNeighborCache::relativeDirectIndices = {
    {{-1, 0}, {0, -1}, {1, 0}, {0, 1}}};

const std::vector<std::pair<int, int>> BoardFieldNeighborCache::relativeDiagonalIndices = {
    {{-1, -1}, {1, -1}, {1, 1}, {-1, 1}}};

const std::vector<std::pair<int, int>> BoardFieldNeighborCache::relativeAllIndices = {
    {{-1, 0}, {0, -1}, {1, 0}, {0, 1}, {-1, -1}, {1, -1}, {1, 1}, {-1, 1}}};

BoardFieldNeighborCache BoardFieldNeighborCache::cache;