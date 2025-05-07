#ifndef BOARD_FIELD_NEIGHBOR_CACHE_H
#define BOARD_FIELD_NEIGHBOR_CACHE_H

#include "BoardIndex.h"

#include <vector>

/**
 * @brief Cache for field neighbors
 *
 */
class BoardFieldNeighborCache {
	struct AllNeighbors {
		std::vector<BoardIndex> absolutDirectNeighborIndices;
		std::vector<BoardIndex> absolutDiagonalNeighborIndices;
		std::vector<BoardIndex> absolutAllNeighborIndices;
	};

	const static std::vector<std::pair<int, int>> relativeDirectIndices;
	const static std::vector<std::pair<int, int>> relativeDiagonalIndices;
	const static std::vector<std::pair<int, int>> relativeAllIndices;

  public:
	static constexpr int rowCount = 4;
	static constexpr int colCount = 5;

	const static auto& GetDirectNeighborIndicesFor(const BoardIndex& ind)
	{
		return cache.cachedNeighbors[ind.row][ind.col].absolutDirectNeighborIndices;
	}

	const static auto& GetDiagonalNeighborIndicesFor(const BoardIndex& ind)
	{
		return cache.cachedNeighbors[ind.row][ind.col].absolutDiagonalNeighborIndices;
	}

	const static auto& GetAllNeighborIndicesFor(const BoardIndex& ind)
	{
		return cache.cachedNeighbors[ind.row][ind.col].absolutAllNeighborIndices;
	}

  private:
	static BoardFieldNeighborCache         cache;
	std::vector<std::vector<AllNeighbors>> cachedNeighbors;

	BoardFieldNeighborCache() : cachedNeighbors(rowCount, std::vector<AllNeighbors>(colCount))
	{
		for (int i = 0; i < rowCount; ++i) {
			for (int j = 0; j < colCount; ++j) {
				LoadAllIndices(i, j, relativeDirectIndices,
				               cachedNeighbors[i][j].absolutDirectNeighborIndices);
				LoadAllIndices(i, j, relativeDiagonalIndices,
				               cachedNeighbors[i][j].absolutDiagonalNeighborIndices);
				LoadAllIndices(i, j, relativeAllIndices,
				               cachedNeighbors[i][j].absolutAllNeighborIndices);
			}
		}
	}

	static void LoadAllIndices(int rowInd, int colInd,
	                           const std::vector<std::pair<int, int>>& relativeIndices,
	                           std::vector<BoardIndex>&                dest)
	{
		for (auto&& relativeIndex : relativeIndices) {
			int newRowInd = rowInd + relativeIndex.first;
			int newColInd = colInd + relativeIndex.second;

			if (IsCorrectInd(newRowInd, newColInd, rowCount, colCount)) {
				dest.push_back(BoardIndex{size_t(newRowInd), size_t(newColInd)});
			}
		}
	}

	static bool IsCorrectInd(int rowInd, int colInd, int maxRow, int maxCol)
	{
		return rowInd >= 0 && rowInd < maxRow && colInd >= 0 && colInd < maxCol;
	}
};

#endif // BOARD_FIELD_NEIGHBOR_CACHE_H