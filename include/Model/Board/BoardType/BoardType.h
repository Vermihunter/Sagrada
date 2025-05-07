#ifndef BOARD_TYPE_H
#define BOARD_TYPE_H

#include "Attributes.h"
#include "Typedefs.h"

#include <unordered_map>
#include <vector>

class Board;
struct BoardIndex;

class BoardTypeNeighborProcessor {
  public:
	virtual ~BoardTypeNeighborProcessor() = default;

	virtual bool is_correct_index(const BoardIndex& ind, const board_config_t& config)  = 0;
	virtual bool are_direct_neighbors(const BoardIndex& ind1, const BoardIndex& ind2)   = 0;
	virtual bool are_diagonal_neighbors(const BoardIndex& ind1, const BoardIndex& ind2) = 0;
	virtual bool are_neighbors(const BoardIndex& ind1, const BoardIndex& ind2)          = 0;
};

struct BoardFieldPlaceableOperator {
	virtual ~BoardFieldPlaceableOperator() = default;

	template <typename Attribute>
	bool any_with_attr(Board& board, const BoardIndex& ind, Attribute attr)
	{
		if constexpr (std::is_same_v<Attribute, color_t>) {
			return any_with_color(board, ind, attr);
		}
		if constexpr (std::is_same_v<Attribute, value_t>) {
			return any_with_value(board, ind, attr);
		}
	}

	virtual bool any_with_color(Board& board, const BoardIndex& atInd, color_t col) = 0;
	virtual bool any_with_value(Board& board, const BoardIndex& atInd, value_t val) = 0;
	virtual bool any_placeable_die(Board& board, const BoardIndex& atInd)           = 0;
};

struct BoardRowParsingFeedBack {
	BoardRowParsingFeedBack(bool _lineCorrect, bool _lastLine)
	    : lineCorrect(_lineCorrect), lastLine(_lastLine)
	{
	}

	bool lineCorrect;
	bool lastLine;
};

struct BoardDataInputHandler {
	virtual ~BoardDataInputHandler() = default;

	virtual BoardRowParsingFeedBack handle_line(const std::string& line) = 0;

  protected:
	BoardDataInputHandler(board_data_t& _boardData, board_config_t _config)
	    : boardData(_boardData), config(_config)
	{
	}

	board_data_t&  boardData;
	board_config_t config;
};

class BoardType {
  public:
	virtual ~BoardType()
	{
		delete neighborProcessor;
		delete placeabilityProcessor;
	}

	static BoardType* get_type(const std::string& operatorName)
	{
		auto operatorIt = definedOperators.find(operatorName);
		if (operatorIt == definedOperators.end()) {
			return nullptr;
		}

		return operatorIt->second;
	}

	BoardTypeNeighborProcessor*  get_neighbor_processor() const { return neighborProcessor; }
	BoardFieldPlaceableOperator* get_placeability_processor() const
	{
		return placeabilityProcessor;
	}

	virtual int                            number_of_fields(const board_config_t config) const = 0;
	virtual const std::vector<BoardIndex>& get_all_indices() const                             = 0;
	virtual std::unique_ptr<BoardDataInputHandler>
	construct_input_handler(board_data_t& _boardData, board_config_t _config) = 0;
	virtual const std::string& get_name() const                               = 0;

	static const auto& get_all_board_types() { return definedOperators; }

  protected:
	inline static std::unordered_map<std::string, BoardType*> definedOperators;
	BoardTypeNeighborProcessor*                               neighborProcessor;
	BoardFieldPlaceableOperator*                              placeabilityProcessor;

	BoardType(const std::string& operatorName, BoardType* concreteOperator,
	          BoardTypeNeighborProcessor*  _neighborProcessor,
	          BoardFieldPlaceableOperator* _placeabilityProcessor)
	    : neighborProcessor(_neighborProcessor), placeabilityProcessor(_placeabilityProcessor)
	{
		definedOperators.insert({operatorName, concreteOperator});
	}
};

#endif // BOARD_TYPE_H