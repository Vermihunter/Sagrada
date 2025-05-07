#ifndef DIAGONAL_CONNECTING_PUBLIC_OBJECTIVE_CARD_H
#define DIAGONAL_CONNECTING_PUBLIC_OBJECTIVE_CARD_H

#include "BoardFieldNeighborCache.h"
#include "PublicObjectiveCard.h"

/**
 * @brief Implementation of the Color Diagonals Public objective card
 * It is defined so adding the future Value Diagonals Public objective card was
 * very easy.
 *
 * @tparam Attribute Currently only used with color_t, but would work perfectly
 * with value_t as well.
 */
template <typename Attribute>
class DiagonalConnectingPublicObjectiveCard : public PublicObjectiveCard {
  public:
	DiagonalConnectingPublicObjectiveCard(concrete_puoc_context_t ctx)
	    : PublicObjectiveCard(std::move(ctx))
	{
	}

	DiagonalConnectingPublicObjectiveCard(const DiagonalConnectingPublicObjectiveCard& other)
	    : PublicObjectiveCard(other.ctx->clone())
	{
	}

	virtual puoc_pattern_t evaluate(OverallBoardState& overallState, board_t& board) override
	{
		puoc_pattern_t puocResultState{*this};
		puocResultState.dicePerPattern = 1;

		BoardFieldPlaceableOperator* placeabilityChecker =
		    board->get_ctx()->config->type->get_placeability_processor();

		BoardIndex ind{0, 0};
		for (auto&& row : board->get_ctx()->data) {
			for (auto&& field : row) {
				Die* die = field->get_die();

				std::optional<Attribute> attr;
				if (die) {
					attr = Die::get_attr<Attribute>(die);
				}

				bool neighborWithSameAttributeExists = false;
				bool anyCorrectPlaceable             = false;
				for (auto&& neighborInd : BoardFieldNeighborCache::GetDiagonalNeighborIndicesFor(
				         ind)) { // neighborProcessor->get_diagonal_neighbor_indices_for(ind,
					             // board->get_ctx()->config)) {
					auto& neighborField = (*board)[neighborInd];

					Die* neighborDie = neighborField->get_die();
					if (!neighborDie) continue;
					Attribute neighborAttribute = Die::get_attr<Attribute>(neighborDie);

					// If there is a die on the field, checking if it is counted
					// as a pattern match
					if (die) {
						if (neighborAttribute == attr.value()) {
							neighborWithSameAttributeExists = true;
							puocResultState.totalEarnedPoints += (ctx->get_satisfaction_value());
							break;
						}
					}
					// If there is no die on a field, we check whether any die
					// with an attribute of a diagonal neighbor's die could be
					// put on the empty field
					else if (!anyCorrectPlaceable) {
						if constexpr (std::is_same_v<Attribute, color_t>) {
							anyCorrectPlaceable =
							    placeabilityChecker->any_with_color(*board, ind, neighborAttribute);
						}
						/*
						if constexpr(std::is_same_v<Attribute, value_t>) {
						        anyCorrectPlaceable =
						placeabilityChecker->any_with_value(*board, ind,
						neighborAttribute);
						}
						*/
					}
				}
				if (!neighborWithSameAttributeExists && anyCorrectPlaceable) {
					puocResultState.completableSubPatterns.push_back({1});
				}
				/*
				if(neighborWithSameAttributeExists) {
				        puocResultState.totalEarnedPoints +=
				(ctx->get_satisfaction_value());
				}
				*/
				++ind.col;
			}
			ind.col = 0;
			++ind.row;
		}

		return puocResultState;
	}

	virtual std::unique_ptr<PublicObjectiveCard> clone() override
	{
		return std::make_unique<DiagonalConnectingPublicObjectiveCard<Attribute>>(*this);
	}
};

#endif // DIAGONAL_CONNECTING_PUBLIC_OBJECTIVE_CARD_H