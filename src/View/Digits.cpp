#include "Digits.h"

DigitInfo::DigitInfo()
{
	digitDrawingCommands.resize(10);

	// ZERO
	digitDrawingCommands[0].push_back(std::make_unique<MoveToCommand>(Point{blockSize / 2, 0}));
	digitDrawingCommands[0].push_back(std::make_unique<LineToCommand>(Point{5 * blockSize / 2, 0}));
	digitDrawingCommands[0].push_back(
	    std::make_unique<LineToCommand>(Point{3 * blockSize, blockSize / 2}));
	digitDrawingCommands[0].push_back(
	    std::make_unique<LineToCommand>(Point{3 * blockSize, 9 * blockSize / 2}));
	digitDrawingCommands[0].push_back(
	    std::make_unique<LineToCommand>(Point{5 * blockSize / 2, 5 * blockSize}));
	digitDrawingCommands[0].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize / 2, 5 * blockSize}));
	digitDrawingCommands[0].push_back(std::make_unique<LineToCommand>(Point{0, 9 * blockSize / 2}));
	digitDrawingCommands[0].push_back(std::make_unique<LineToCommand>(Point{0, blockSize / 2}));
	digitDrawingCommands[0].push_back(std::make_unique<LineToCommand>(Point{blockSize / 2, 0}));

	digitDrawingCommands[0].push_back(std::make_unique<MoveToCommand>(Point{blockSize, blockSize}));
	digitDrawingCommands[0].push_back(
	    std::make_unique<LineToCommand>(Point{2 * blockSize, blockSize}));
	digitDrawingCommands[0].push_back(
	    std::make_unique<LineToCommand>(Point{2 * blockSize, 4 * blockSize}));
	digitDrawingCommands[0].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize, 4 * blockSize}));
	digitDrawingCommands[0].push_back(std::make_unique<LineToCommand>(Point{blockSize, blockSize}));

	// ONE
	digitDrawingCommands[1].push_back(std::make_unique<MoveToCommand>(Point{blockSize, 0}));
	digitDrawingCommands[1].push_back(std::make_unique<LineToCommand>(Point{2 * blockSize, 0}));
	digitDrawingCommands[1].push_back(
	    std::make_unique<LineToCommand>(Point{2 * blockSize, 4 * blockSize}));
	digitDrawingCommands[1].push_back(
	    std::make_unique<LineToCommand>(Point{3 * blockSize, 4 * blockSize}));
	digitDrawingCommands[1].push_back(
	    std::make_unique<LineToCommand>(Point{3 * blockSize, 5 * blockSize}));
	digitDrawingCommands[1].push_back(std::make_unique<LineToCommand>(Point{0, 5 * blockSize}));
	digitDrawingCommands[1].push_back(std::make_unique<LineToCommand>(Point{0, 4 * blockSize}));
	digitDrawingCommands[1].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize, 4 * blockSize}));
	digitDrawingCommands[1].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize, blockSize + 8}));
	digitDrawingCommands[1].push_back(std::make_unique<LineToCommand>(Point{0, 2 * blockSize + 8}));
	digitDrawingCommands[1].push_back(std::make_unique<LineToCommand>(Point{0, blockSize}));
	digitDrawingCommands[1].push_back(std::make_unique<LineToCommand>(Point{blockSize, 0}));

	// TWO
	digitDrawingCommands[2].push_back(std::make_unique<MoveToCommand>(Point{0, blockSize / 2}));
	digitDrawingCommands[2].push_back(std::make_unique<LineToCommand>(Point{blockSize / 2, 0}));
	digitDrawingCommands[2].push_back(std::make_unique<LineToCommand>(Point{5 * blockSize / 2, 0}));
	digitDrawingCommands[2].push_back(
	    std::make_unique<LineToCommand>(Point{3 * blockSize, blockSize / 2}));
	digitDrawingCommands[2].push_back(
	    std::make_unique<LineToCommand>(Point{3 * blockSize, 5 * blockSize / 2}));
	digitDrawingCommands[2].push_back(
	    std::make_unique<LineToCommand>(Point{5 * blockSize / 2, 3 * blockSize}));
	digitDrawingCommands[2].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize, 3 * blockSize}));
	digitDrawingCommands[2].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize, 4 * blockSize}));
	digitDrawingCommands[2].push_back(
	    std::make_unique<LineToCommand>(Point{2 * blockSize, 4 * blockSize}));
	digitDrawingCommands[2].push_back(
	    std::make_unique<LineToCommand>(Point{2 * blockSize, 7 * blockSize / 2}));
	digitDrawingCommands[2].push_back(
	    std::make_unique<LineToCommand>(Point{3 * blockSize, 7 * blockSize / 2}));
	digitDrawingCommands[2].push_back(
	    std::make_unique<LineToCommand>(Point{3 * blockSize, 5 * blockSize}));
	digitDrawingCommands[2].push_back(std::make_unique<LineToCommand>(Point{0, 5 * blockSize}));
	digitDrawingCommands[2].push_back(std::make_unique<LineToCommand>(Point{0, 5 * blockSize / 2}));
	digitDrawingCommands[2].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize / 2, 2 * blockSize}));
	digitDrawingCommands[2].push_back(
	    std::make_unique<LineToCommand>(Point{2 * blockSize, 2 * blockSize}));
	digitDrawingCommands[2].push_back(
	    std::make_unique<LineToCommand>(Point{2 * blockSize, blockSize}));
	digitDrawingCommands[2].push_back(std::make_unique<LineToCommand>(Point{blockSize, blockSize}));
	digitDrawingCommands[2].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize, 3 * blockSize / 2}));
	digitDrawingCommands[2].push_back(std::make_unique<LineToCommand>(Point{0, 3 * blockSize / 2}));
	digitDrawingCommands[2].push_back(std::make_unique<LineToCommand>(Point{0, blockSize / 2}));

	// THREE
	digitDrawingCommands[3].push_back(std::make_unique<MoveToCommand>(Point{0, blockSize / 2}));
	digitDrawingCommands[3].push_back(std::make_unique<LineToCommand>(Point{blockSize / 2, 0}));
	digitDrawingCommands[3].push_back(std::make_unique<LineToCommand>(Point{5 * blockSize / 2, 0}));
	digitDrawingCommands[3].push_back(
	    std::make_unique<LineToCommand>(Point{3 * blockSize, blockSize / 2}));
	digitDrawingCommands[3].push_back(
	    std::make_unique<LineToCommand>(Point{3 * blockSize, 2 * blockSize}));
	digitDrawingCommands[3].push_back(
	    std::make_unique<LineToCommand>(Point{5 * blockSize / 2, 5 * blockSize / 2}));
	digitDrawingCommands[3].push_back(
	    std::make_unique<LineToCommand>(Point{3 * blockSize, 3 * blockSize}));
	digitDrawingCommands[3].push_back(
	    std::make_unique<LineToCommand>(Point{3 * blockSize, 9 * blockSize / 2}));
	digitDrawingCommands[3].push_back(
	    std::make_unique<LineToCommand>(Point{5 * blockSize / 2, 5 * blockSize}));
	digitDrawingCommands[3].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize / 2, 5 * blockSize}));
	digitDrawingCommands[3].push_back(std::make_unique<LineToCommand>(Point{0, 9 * blockSize / 2}));
	digitDrawingCommands[3].push_back(std::make_unique<LineToCommand>(Point{0, 7 * blockSize / 2}));
	digitDrawingCommands[3].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize, 7 * blockSize / 2}));
	digitDrawingCommands[3].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize, 4 * blockSize}));
	digitDrawingCommands[3].push_back(
	    std::make_unique<LineToCommand>(Point{2 * blockSize, 4 * blockSize}));
	digitDrawingCommands[3].push_back(
	    std::make_unique<LineToCommand>(Point{2 * blockSize, 3 * blockSize}));
	digitDrawingCommands[3].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize, 3 * blockSize}));
	digitDrawingCommands[3].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize, 2 * blockSize}));
	digitDrawingCommands[3].push_back(
	    std::make_unique<LineToCommand>(Point{2 * blockSize, 2 * blockSize}));
	digitDrawingCommands[3].push_back(
	    std::make_unique<LineToCommand>(Point{2 * blockSize, blockSize}));
	digitDrawingCommands[3].push_back(std::make_unique<LineToCommand>(Point{blockSize, blockSize}));
	digitDrawingCommands[3].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize, 3 * blockSize / 2}));
	digitDrawingCommands[3].push_back(std::make_unique<LineToCommand>(Point{0, 3 * blockSize / 2}));
	digitDrawingCommands[3].push_back(std::make_unique<LineToCommand>(Point{0, blockSize / 2}));

	// FOUR
	digitDrawingCommands[4].push_back(std::make_unique<MoveToCommand>(Point{3 * blockSize / 2, 0}));
	digitDrawingCommands[4].push_back(std::make_unique<LineToCommand>(Point{5 * blockSize / 2, 0}));
	digitDrawingCommands[4].push_back(
	    std::make_unique<LineToCommand>(Point{5 * blockSize / 2, 5 * blockSize / 2}));
	digitDrawingCommands[4].push_back(
	    std::make_unique<LineToCommand>(Point{3 * blockSize, 5 * blockSize / 2}));
	digitDrawingCommands[4].push_back(
	    std::make_unique<LineToCommand>(Point{3 * blockSize, 7 * blockSize / 2}));
	digitDrawingCommands[4].push_back(
	    std::make_unique<LineToCommand>(Point{5 * blockSize / 2, 7 * blockSize / 2}));
	digitDrawingCommands[4].push_back(
	    std::make_unique<LineToCommand>(Point{5 * blockSize / 2, 7 * blockSize / 2}));
	digitDrawingCommands[4].push_back(
	    std::make_unique<LineToCommand>(Point{5 * blockSize / 2, 4 * blockSize}));
	digitDrawingCommands[4].push_back(
	    std::make_unique<LineToCommand>(Point{3 * blockSize, 4 * blockSize}));
	digitDrawingCommands[4].push_back(
	    std::make_unique<LineToCommand>(Point{3 * blockSize, 5 * blockSize}));
	digitDrawingCommands[4].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize, 5 * blockSize}));
	digitDrawingCommands[4].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize, 4 * blockSize}));
	digitDrawingCommands[4].push_back(
	    std::make_unique<LineToCommand>(Point{3 * blockSize / 2, 4 * blockSize}));
	digitDrawingCommands[4].push_back(
	    std::make_unique<LineToCommand>(Point{3 * blockSize / 2, 7 * blockSize / 2}));
	digitDrawingCommands[4].push_back(std::make_unique<LineToCommand>(Point{0, 7 * blockSize / 2}));
	digitDrawingCommands[4].push_back(std::make_unique<LineToCommand>(Point{0, 5 * blockSize / 2}));
	digitDrawingCommands[4].push_back(std::make_unique<LineToCommand>(Point{3 * blockSize / 2, 0}));

	digitDrawingCommands[4].push_back(
	    std::make_unique<MoveToCommand>(Point{blockSize, 5 * blockSize / 2}));
	digitDrawingCommands[4].push_back(
	    std::make_unique<LineToCommand>(Point{3 * blockSize / 2, 2 * blockSize}));
	digitDrawingCommands[4].push_back(
	    std::make_unique<LineToCommand>(Point{3 * blockSize / 2, 5 * blockSize / 2}));
	digitDrawingCommands[4].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize, 5 * blockSize / 2}));

	// FIVE
	digitDrawingCommands[5].push_back(std::make_unique<MoveToCommand>(Point{0, 0}));
	digitDrawingCommands[5].push_back(std::make_unique<LineToCommand>(Point{3 * blockSize, 0}));
	digitDrawingCommands[5].push_back(
	    std::make_unique<LineToCommand>(Point{3 * blockSize, 3 * blockSize / 2}));
	digitDrawingCommands[5].push_back(
	    std::make_unique<LineToCommand>(Point{2 * blockSize, 3 * blockSize / 2}));
	digitDrawingCommands[5].push_back(
	    std::make_unique<LineToCommand>(Point{2 * blockSize, blockSize}));
	digitDrawingCommands[5].push_back(std::make_unique<LineToCommand>(Point{blockSize, blockSize}));
	digitDrawingCommands[5].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize, 2 * blockSize}));
	digitDrawingCommands[5].push_back(
	    std::make_unique<LineToCommand>(Point{5 * blockSize / 2, 2 * blockSize}));
	digitDrawingCommands[5].push_back(
	    std::make_unique<LineToCommand>(Point{3 * blockSize, 5 * blockSize / 2}));
	digitDrawingCommands[5].push_back(
	    std::make_unique<LineToCommand>(Point{3 * blockSize, 9 * blockSize / 2}));
	digitDrawingCommands[5].push_back(
	    std::make_unique<LineToCommand>(Point{5 * blockSize / 2, 5 * blockSize}));
	digitDrawingCommands[5].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize / 2, 5 * blockSize}));
	digitDrawingCommands[5].push_back(std::make_unique<LineToCommand>(Point{0, 9 * blockSize / 2}));
	digitDrawingCommands[5].push_back(std::make_unique<LineToCommand>(Point{0, 7 * blockSize / 2}));
	digitDrawingCommands[5].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize, 7 * blockSize / 2}));
	digitDrawingCommands[5].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize, 4 * blockSize}));
	digitDrawingCommands[5].push_back(
	    std::make_unique<LineToCommand>(Point{2 * blockSize, 4 * blockSize}));
	digitDrawingCommands[5].push_back(
	    std::make_unique<LineToCommand>(Point{2 * blockSize, 3 * blockSize}));
	digitDrawingCommands[5].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize / 2, 3 * blockSize}));
	digitDrawingCommands[5].push_back(std::make_unique<LineToCommand>(Point{0, 5 * blockSize / 2}));
	digitDrawingCommands[5].push_back(std::make_unique<LineToCommand>(Point{0, 0}));

	// SIX
	digitDrawingCommands[6].push_back(std::make_unique<MoveToCommand>(Point{blockSize / 2, 0}));
	digitDrawingCommands[6].push_back(std::make_unique<LineToCommand>(Point{5 * blockSize / 2, 0}));
	digitDrawingCommands[6].push_back(
	    std::make_unique<LineToCommand>(Point{3 * blockSize, blockSize / 2}));
	digitDrawingCommands[6].push_back(
	    std::make_unique<LineToCommand>(Point{3 * blockSize, 3 * blockSize / 2}));
	digitDrawingCommands[6].push_back(
	    std::make_unique<LineToCommand>(Point{2 * blockSize, 3 * blockSize / 2}));
	digitDrawingCommands[6].push_back(
	    std::make_unique<LineToCommand>(Point{2 * blockSize, blockSize}));
	digitDrawingCommands[6].push_back(std::make_unique<LineToCommand>(Point{blockSize, blockSize}));
	digitDrawingCommands[6].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize, 2 * blockSize}));
	digitDrawingCommands[6].push_back(
	    std::make_unique<LineToCommand>(Point{5 * blockSize / 2, 2 * blockSize}));
	digitDrawingCommands[6].push_back(
	    std::make_unique<LineToCommand>(Point{3 * blockSize, 5 * blockSize / 2}));
	digitDrawingCommands[6].push_back(
	    std::make_unique<LineToCommand>(Point{3 * blockSize, 9 * blockSize / 2}));
	digitDrawingCommands[6].push_back(
	    std::make_unique<LineToCommand>(Point{5 * blockSize / 2, 5 * blockSize}));
	digitDrawingCommands[6].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize / 2, 5 * blockSize}));
	digitDrawingCommands[6].push_back(std::make_unique<LineToCommand>(Point{0, 9 * blockSize / 2}));
	digitDrawingCommands[6].push_back(std::make_unique<LineToCommand>(Point{0, blockSize / 2}));
	digitDrawingCommands[6].push_back(std::make_unique<LineToCommand>(Point{blockSize / 2, 0}));

	digitDrawingCommands[6].push_back(
	    std::make_unique<MoveToCommand>(Point{blockSize, 3 * blockSize}));
	digitDrawingCommands[6].push_back(
	    std::make_unique<LineToCommand>(Point{2 * blockSize, 3 * blockSize}));
	digitDrawingCommands[6].push_back(
	    std::make_unique<LineToCommand>(Point{2 * blockSize, 4 * blockSize}));
	digitDrawingCommands[6].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize, 4 * blockSize}));
	digitDrawingCommands[6].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize, 3 * blockSize}));

	// SEVEN
	digitDrawingCommands[7].push_back(std::make_unique<MoveToCommand>(Point{0, 0}));
	digitDrawingCommands[7].push_back(std::make_unique<LineToCommand>(Point{3 * blockSize, 0}));
	digitDrawingCommands[7].push_back(
	    std::make_unique<LineToCommand>(Point{3 * blockSize, blockSize}));
	digitDrawingCommands[7].push_back(
	    std::make_unique<LineToCommand>(Point{2 * blockSize, 4 * blockSize}));
	digitDrawingCommands[7].push_back(
	    std::make_unique<LineToCommand>(Point{5 * blockSize / 2, 4 * blockSize}));
	digitDrawingCommands[7].push_back(
	    std::make_unique<LineToCommand>(Point{5 * blockSize / 2, 5 * blockSize}));
	digitDrawingCommands[7].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize / 2, 5 * blockSize}));
	digitDrawingCommands[7].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize / 2, 4 * blockSize}));
	digitDrawingCommands[7].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize, 4 * blockSize}));
	digitDrawingCommands[7].push_back(
	    std::make_unique<LineToCommand>(Point{2 * blockSize, blockSize}));
	digitDrawingCommands[7].push_back(std::make_unique<LineToCommand>(Point{blockSize, blockSize}));
	digitDrawingCommands[7].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize, 3 * blockSize / 2}));
	digitDrawingCommands[7].push_back(std::make_unique<LineToCommand>(Point{0, 3 * blockSize / 2}));
	digitDrawingCommands[7].push_back(std::make_unique<LineToCommand>(Point{0, 0}));

	// EIGHT
	digitDrawingCommands[8].push_back(std::make_unique<MoveToCommand>(Point{blockSize / 2, 0}));
	digitDrawingCommands[8].push_back(std::make_unique<LineToCommand>(Point{5 * blockSize / 2, 0}));
	digitDrawingCommands[8].push_back(
	    std::make_unique<LineToCommand>(Point{3 * blockSize, blockSize / 2}));
	digitDrawingCommands[8].push_back(
	    std::make_unique<LineToCommand>(Point{3 * blockSize, 2 * blockSize}));
	digitDrawingCommands[8].push_back(
	    std::make_unique<LineToCommand>(Point{5 * blockSize / 2, 5 * blockSize / 2}));
	digitDrawingCommands[8].push_back(
	    std::make_unique<LineToCommand>(Point{3 * blockSize, 3 * blockSize}));
	digitDrawingCommands[8].push_back(
	    std::make_unique<LineToCommand>(Point{3 * blockSize, 9 * blockSize / 2}));
	digitDrawingCommands[8].push_back(
	    std::make_unique<LineToCommand>(Point{5 * blockSize / 2, 5 * blockSize}));
	digitDrawingCommands[8].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize / 2, 5 * blockSize}));
	digitDrawingCommands[8].push_back(std::make_unique<LineToCommand>(Point{0, 9 * blockSize / 2}));
	digitDrawingCommands[8].push_back(std::make_unique<LineToCommand>(Point{0, 3 * blockSize}));
	digitDrawingCommands[8].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize / 2, 5 * blockSize / 2}));
	digitDrawingCommands[8].push_back(std::make_unique<LineToCommand>(Point{0, 2 * blockSize}));
	digitDrawingCommands[8].push_back(std::make_unique<LineToCommand>(Point{0, blockSize / 2}));
	digitDrawingCommands[8].push_back(std::make_unique<LineToCommand>(Point{blockSize / 2, 0}));

	digitDrawingCommands[8].push_back(std::make_unique<MoveToCommand>(Point{blockSize, blockSize}));
	digitDrawingCommands[8].push_back(
	    std::make_unique<LineToCommand>(Point{2 * blockSize, blockSize}));
	digitDrawingCommands[8].push_back(
	    std::make_unique<LineToCommand>(Point{2 * blockSize, 2 * blockSize}));
	digitDrawingCommands[8].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize, 2 * blockSize}));
	digitDrawingCommands[8].push_back(std::make_unique<LineToCommand>(Point{blockSize, blockSize}));

	digitDrawingCommands[8].push_back(
	    std::make_unique<MoveToCommand>(Point{blockSize, 3 * blockSize}));
	digitDrawingCommands[8].push_back(
	    std::make_unique<LineToCommand>(Point{2 * blockSize, 3 * blockSize}));
	digitDrawingCommands[8].push_back(
	    std::make_unique<LineToCommand>(Point{2 * blockSize, 4 * blockSize}));
	digitDrawingCommands[8].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize, 4 * blockSize}));
	digitDrawingCommands[8].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize, 3 * blockSize}));

	// NINE
	digitDrawingCommands[9].push_back(std::make_unique<MoveToCommand>(Point{blockSize / 2, 0}));
	digitDrawingCommands[9].push_back(std::make_unique<LineToCommand>(Point{5 * blockSize / 2, 0}));
	digitDrawingCommands[9].push_back(
	    std::make_unique<LineToCommand>(Point{3 * blockSize, blockSize / 2}));
	digitDrawingCommands[9].push_back(
	    std::make_unique<LineToCommand>(Point{3 * blockSize, 9 * blockSize / 2}));
	digitDrawingCommands[9].push_back(
	    std::make_unique<LineToCommand>(Point{5 * blockSize / 2, 5 * blockSize}));
	digitDrawingCommands[9].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize / 2, 5 * blockSize}));
	digitDrawingCommands[9].push_back(std::make_unique<LineToCommand>(Point{0, 9 * blockSize / 2}));
	digitDrawingCommands[9].push_back(std::make_unique<LineToCommand>(Point{0, 7 * blockSize / 2}));
	digitDrawingCommands[9].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize, 7 * blockSize / 2}));
	digitDrawingCommands[9].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize, 4 * blockSize}));
	digitDrawingCommands[9].push_back(
	    std::make_unique<LineToCommand>(Point{2 * blockSize, 4 * blockSize}));
	digitDrawingCommands[9].push_back(
	    std::make_unique<LineToCommand>(Point{2 * blockSize, 3 * blockSize}));
	digitDrawingCommands[9].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize / 2, 3 * blockSize}));
	digitDrawingCommands[9].push_back(std::make_unique<LineToCommand>(Point{0, 5 * blockSize / 2}));
	digitDrawingCommands[9].push_back(std::make_unique<LineToCommand>(Point{0, blockSize / 2}));
	digitDrawingCommands[9].push_back(std::make_unique<LineToCommand>(Point{blockSize / 2, 0}));

	digitDrawingCommands[9].push_back(std::make_unique<MoveToCommand>(Point{blockSize, blockSize}));
	digitDrawingCommands[9].push_back(
	    std::make_unique<LineToCommand>(Point{2 * blockSize, blockSize}));
	digitDrawingCommands[9].push_back(
	    std::make_unique<LineToCommand>(Point{2 * blockSize, 2 * blockSize}));
	digitDrawingCommands[9].push_back(
	    std::make_unique<LineToCommand>(Point{blockSize, 2 * blockSize}));
	digitDrawingCommands[9].push_back(std::make_unique<LineToCommand>(Point{blockSize, blockSize}));
}
