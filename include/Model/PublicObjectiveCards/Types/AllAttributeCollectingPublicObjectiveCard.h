#ifndef ALL_VALUE_COLLECTING_PUBLIC_OBJECTIVE_CARD_H
#define ALL_VALUE_COLLECTING_PUBLIC_OBJECTIVE_CARD_H

#include "PublicObjectiveCard.h"

#include <array>

/**
 * @brief The class implementing Public objective cards Color Variety and Shade
 * Variety The implementation differs only on the attribute that has to be
 * collected.
 * @tparam Attribute The attribute that has to be collected in the Puoc pattern.
 * Either color_t or value_t
 */
template <typename Attribute>
class AllAttributeCollectingPublicObjectiveCard : public PublicObjectiveCard {
	constexpr static size_t attr_size = size_t(Attribute::COUNT);
	using attribute_tracker_t         = std::array<size_t, attr_size + 1>;

  public:
	AllAttributeCollectingPublicObjectiveCard(concrete_puoc_context_t ctx)
	    : PublicObjectiveCard(std::move(ctx))
	{
	}

	AllAttributeCollectingPublicObjectiveCard(
	    const AllAttributeCollectingPublicObjectiveCard& other)
	    : PublicObjectiveCard(other.ctx->clone())
	{
	}

	virtual puoc_pattern_t evaluate(OverallBoardState& overallState, board_t& board) override
	{
		puoc_pattern_t puocResultState{*this};
		puocResultState.dicePerPattern = attr_size;

		attribute_tracker_t placeableAttributeCounter;
		placeableAttributeCounter.fill(0);
		attribute_tracker_t attributeCounter;
		attributeCounter.fill(0);

		BoardIndex ind{0, 0};
		for (auto&& row : board->get_ctx()->data) {
			for (auto&& field : row) {
				Die*   die = field->get_die();
				size_t attributeInd;
				// Field has no die on it -> incrementing the last element
				// specified for empty fields
				if (!die) {
					//	add_placeability_processing(board, ind,
					// placeableAttributeCounter);
				}
				// Otherwise, incrementing the element at the correct ind
				else {
					attributeInd = size_t(Die::get_attr<Attribute>(die));
					attributeCounter[attributeInd] += 1;
				}
				++ind.col;
			}
			ind.col = 0;
			++ind.row;
		}

		size_t maxPatternCount =
		    board->get_ctx()->config->type->number_of_fields(board->get_ctx()->config) / attr_size;
		for (size_t i = 0; i < maxPatternCount; ++i) {
			bool patternMatched   = true;
			bool patternMatchable = true;
			int  diceToMatch      = 0;
			for (size_t i = 0; i < attr_size; ++i) {
				if (attributeCounter[i] > 0) {
					--attributeCounter[i];
				}
				else if (placeableAttributeCounter[i] > 0) {
					patternMatched = false;
					--placeableAttributeCounter[i];
					++diceToMatch;
				}
				else {
					patternMatchable = false;
					break;
				}
			}

			if (patternMatched) {
				puocResultState.totalEarnedPoints += (ctx->get_satisfaction_value());
			}
			else if (patternMatchable) {
				puocResultState.completableSubPatterns.push_back({.diceToComplete = diceToMatch});
			}
			else {
				++puocResultState.uncompletableSubPatternCount;
			}
		}

		return puocResultState;
	}

	virtual std::unique_ptr<PublicObjectiveCard> clone() override
	{
		return std::make_unique<AllAttributeCollectingPublicObjectiveCard<Attribute>>(*this);
	}

  private:
	void add_placeability_processing(board_t& board, const BoardIndex& ind,
	                                 attribute_tracker_t& placeableCounter)
	{
		BoardFieldPlaceableOperator* placeabilityChecker =
		    board->get_ctx()->config->type->get_placeability_processor();
		for (size_t i = 0; i < size_t(Attribute::COUNT); ++i) {
			Attribute attr = Attribute(i);
			if (placeabilityChecker->any_with_attr(*board, ind, attr)) {
				++placeableCounter[i];
			}
		}
	}
};

#endif // ALL_VALUE_COLLECTING_PUBLIC_OBJECTIVE_CARD_H