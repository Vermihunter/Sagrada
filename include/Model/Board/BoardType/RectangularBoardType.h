#ifndef RECTANGULAR_BOARD_TYPE_H
#define RECTANGULAR_BOARD_TYPE_H

#include "Board.h"
#include "BoardFieldNeighborCache.h"
// #include "BoardReader.h"
#include "BoardType.h"

/**
 * @brief Basic mapping function
 *
 * @param c Input character
 * @return restriction_t Output restriction container
 */
inline restriction_t char_to_restriction(char c)
{
	auto restr = std::make_unique<BoardFieldRestrictions>();

	switch (c) {
	// Characters indicating color
	case 'Y':
		restr->colorRestriction = color_t::YELLOW;
		break;
	case 'G':
		restr->colorRestriction = color_t::GREEN;
		break;
	case 'B':
		restr->colorRestriction = color_t::BLUE;
		break;
	case 'P':
		restr->colorRestriction = color_t::PURPLE;
		break;
	case 'R':
		restr->colorRestriction = color_t::RED;
		break;

	// Characters indicating value
	case '1':
		restr->valueRestriction = value_t::ONE;
		break;
	case '2':
		restr->valueRestriction = value_t::TWO;
		break;
	case '3':
		restr->valueRestriction = value_t::THREE;
		break;
	case '4':
		restr->valueRestriction = value_t::FOUR;
		break;
	case '5':
		restr->valueRestriction = value_t::FIVE;
		break;
	case '6':
		restr->valueRestriction = value_t::SIX;
		break;

	// Character indicates field without restriction
	case '_':
		break;
	// Invalid character
	default:
		// Log something bad
		break;
	}

	return restr;
}

class RectangularBoardTypeNeighborProcessor : public BoardTypeNeighborProcessor {
	using relative_ind_c = const std::array<std::pair<int, int>, 4>;

  public:
	virtual bool is_correct_index(const BoardIndex& ind, const board_config_t& config) override
	{
		return ind.row < config->height && ind.col < config->width;
	}

	virtual bool are_direct_neighbors(const BoardIndex& ind1, const BoardIndex& ind2) override
	{
		return get_ind_abs(ind1, ind2) == 1;
	}

	virtual bool are_diagonal_neighbors(const BoardIndex& ind1, const BoardIndex& ind2) override
	{
		return get_ind_abs(ind1, ind2) == 2;
	}

	virtual bool are_neighbors(const BoardIndex& ind1, const BoardIndex& ind2) override
	{
		return are_direct_neighbors(ind1, ind2) || are_diagonal_neighbors(ind1, ind2);
	}

  private:
	int get_ind_abs(const BoardIndex& ind1, const BoardIndex& ind2)
	{
		int iInd1Row = int(ind1.row);
		int iInd1Col = int(ind1.col);

		int iInd2Row = int(ind2.row);
		int iInd2Col = int(ind2.col);

		int rowRes = std::abs(iInd1Row - iInd2Row);
		int colRes = std::abs(iInd1Col - iInd2Col);

		return rowRes + colRes;
	}

	template <size_t N>
	std::vector<BoardIndex>
	get_neighbor_indices_impl(const BoardIndex& ind, const board_config_t& config,
	                          const std::array<std::pair<int, int>, N>& relativeIndContainer)
	{
		std::vector<BoardIndex> res;
		int                     iIndRow = int(ind.row);
		int                     iIndCol = int(ind.col);

		for (auto&& [iNeighborRow, iNeighborCol] : relativeIndContainer) {
			int iAbsoluteNeighborRow = iNeighborRow + iIndRow;
			int iAbsoluteNeighborCol = iNeighborCol + iIndCol;

			if (iAbsoluteNeighborCol < 0 || iAbsoluteNeighborRow < 0) continue;

			BoardIndex newInd{size_t(iAbsoluteNeighborRow), size_t(iAbsoluteNeighborCol)};
			if (!is_correct_index(newInd, config)) continue;

			res.push_back(newInd);
		}

		return res;
	}
};

struct RectangleBoardDataInputHandler : BoardDataInputHandler {
	RectangleBoardDataInputHandler(board_data_t& data, board_config_t config)
	    : BoardDataInputHandler(data, config), currLine(0)
	{
	}

	virtual BoardRowParsingFeedBack handle_line(const std::string& line) override
	{
		if (currLine >= config->height) {
			return BoardRowParsingFeedBack{false, false};
		}

		boardData.push_back(std::vector<board_field_t>{});
		bool   lineCorrect = true;
		size_t colInd      = 0;
		for (auto&& c : line) {
			BoardIndex newInd{currLine, colInd};
			auto       restr = char_to_restriction(c);
			boardData[currLine].push_back(new BoardField(std::move(restr)));

			++colInd;
		}

		++currLine;
		return BoardRowParsingFeedBack{lineCorrect, /* lastLine = */ (currLine == config->height)};
	}

  private:
	size_t currLine;
};

struct RectangleBoardFieldPlaceableOperator : BoardFieldPlaceableOperator {
	/**
	 * @brief Checks if any die could be put on a given field
	 * If the field already contains a die, false is returned,
	 * otherwise the only way no die could be put on a field is
	 * when a direct neighbor contains a die with same attribute
	 * (color or value) as the current field's restriction.
	 *
	 * @param board
	 * @param ind
	 * @return true At least one die could be put on the field
	 * @return false No die could be put on the field
	 */
	virtual bool any_placeable_die(Board& board, const BoardIndex& ind) override
	{
		auto& field = board[ind];
		// If the field already contains a die, no other die could be put there
		if (field->get_die() != nullptr) {
			return false;
		}

		auto value_restriction = field->get_value_restriction();
		auto color_restriction = field->get_color_restriction();
		// It is impossible to have a field without any restriction and not be
		// able to place a die
		if (!value_restriction.has_value() && !color_restriction.has_value()) {
			return true;
		}

		bool res;
		if (value_restriction.has_value()) {
			res = check_for_colliding(board, ind, value_restriction.value());
		}
		else {
			res = check_for_colliding(board, ind, color_restriction.value());
		}
		return res;
	}

	virtual bool any_with_color(Board& board, const BoardIndex& atInd, color_t col) override
	{
		return any_with_attribute(board, atInd, col);
	}

	virtual bool any_with_value(Board& board, const BoardIndex& atInd, value_t val) override
	{
		return any_with_attribute(board, atInd, val);
	}

  private:
	template <typename Attribute>
	bool any_with_attribute(Board& board, const BoardIndex& ind, Attribute attr)
	{
		auto& field = board[ind];
		if (field->get_die()) {
			return false;
		}

		std::optional<Attribute> attr_restriction = field->get_attr_restriction<Attribute>();
		// The field has a different restriction defined
		if (attr_restriction.has_value() && attr_restriction.value() != attr) {
			return false;
		}

		return check_for_colliding(board, ind, attr);
	}

	template <typename Attribute>
	bool check_for_colliding(Board& board, const BoardIndex& ind, Attribute attr)
	{
		for (auto&& neighborInd : BoardFieldNeighborCache::GetDirectNeighborIndicesFor(ind)) {
			auto& neighborField = board[neighborInd];
			Die*  neighborDie   = neighborField->get_die();
			if (!neighborDie) continue;

			// One of the dice' neighbors has the same attribute as the searched
			// one
			if (attr == Die::get_attr<Attribute>(neighborDie)) return false;
		}

		return true;
	}
};

class RectangularBoardType : public BoardType {
	// static RectangularBoardType singletonInstance;
  public:
	virtual std::unique_ptr<BoardDataInputHandler>
	construct_input_handler(board_data_t& boardData, board_config_t config) override
	{
		return std::make_unique<RectangleBoardDataInputHandler>(boardData, config);
	}

	static BoardType* get()
	{
		static RectangularBoardType singletonInstance;
		return &singletonInstance;
	}

	virtual int number_of_fields(const board_config_t config) const override
	{
		return config->height * config->width;
	}

	virtual const std::vector<BoardIndex>& get_all_indices() const { return allIndices; }

	virtual const std::string& get_name() const override { return name; }

  private:
	static inline const std::string name = "Rectangle";
	std::vector<BoardIndex>         allIndices;

	RectangularBoardType()
	    : BoardType(name, this, new RectangularBoardTypeNeighborProcessor(),
	                new RectangleBoardFieldPlaceableOperator())
	{
		// TODO: hard coded
		for (size_t i = 0; i < 4; ++i) {
			for (size_t j = 0; j < 5; ++j) {
				allIndices.push_back({i, j});
			}
		}
	}
};

#endif // RECTANGULAR_BOARD_TYPE_H