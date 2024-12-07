#ifndef TWO_ATTRIBUTE_COLLECTING_PUBLIC_OBJECTIVE_CARD_H
#define TWO_ATTRIBUTE_COLLECTING_PUBLIC_OBJECTIVE_CARD_H

#include "PublicObjectiveCard.h"

/**
 * @brief Implementation of Light Shades, Medium Shades and Deep Shades Public
 * objective cards The two concrete attributes are passed to the object at
 * construction.
 * @tparam Attribute Currently only value_t is used, but it would perfectly fit
 * with color_t and two color implementation.
 */
template <typename Attribute>
class TwoAttributeCollectingPublicObjectiveCard : public PublicObjectiveCard
{
  public:
    TwoAttributeCollectingPublicObjectiveCard(concrete_puoc_context_t ctx,
                                              Attribute _attr1,
                                              Attribute _attr2)
        : PublicObjectiveCard(std::move(ctx)), attr1(_attr1), attr2(_attr2)
    {
    }

    TwoAttributeCollectingPublicObjectiveCard(
        const TwoAttributeCollectingPublicObjectiveCard& other)
        : PublicObjectiveCard(other.ctx->clone())
        , attr1(other.attr1)
        , attr2(other.attr2)
    {
    }

    virtual puoc_pattern_t evaluate(OverallBoardState& overallState,
                                    board_t& board) override
    {
        puoc_pattern_t puocResultState {*this};
        puocResultState.dicePerPattern = 2;

        int attr1Count = 0;
        int attr2Count = 0;

        int totalFieldCount =
            board->get_ctx()->config->height * board->get_ctx()->config->width;
        int attr1PlaceableToEmpty = totalFieldCount;
        int attr2PlaceableToEmpty = totalFieldCount;

        std::vector<std::pair<bool, bool>> placeableAttributeCache(
            totalFieldCount, std::pair {true, true});

        BoardIndex ind {0, 0};
        for (auto&& row : board->get_ctx()->data) {
            for (auto&& field : row) {
                Die* die = field->get_die();
                if (!die) {
                    ++ind.col;
                    continue;
                }

                Attribute fieldAttribute = Die::get_attr<Attribute>(die);

                if (fieldAttribute == attr1) {
                    ++attr1Count;
                    for (auto&& directNeighborInd :
                         BoardFieldNeighborCache::GetDirectNeighborIndicesFor(
                             ind)) {
                        if (placeableAttributeCache[directNeighborInd.col *
                                                    directNeighborInd.row]
                                .first) {
                            --attr1PlaceableToEmpty;
                            placeableAttributeCache[directNeighborInd.col *
                                                    directNeighborInd.row]
                                .first = false;
                        }
                    }
                }
                else if (fieldAttribute == attr2) {
                    ++attr2Count;
                    for (auto&& directNeighborInd :
                         BoardFieldNeighborCache::GetDirectNeighborIndicesFor(
                             ind)) {
                        if (placeableAttributeCache[directNeighborInd.col *
                                                    directNeighborInd.row]
                                .second) {
                            --attr2PlaceableToEmpty;
                            placeableAttributeCache[directNeighborInd.col *
                                                    directNeighborInd.row]
                                .second = false;
                        }
                    }
                }

                ++ind.col;
            }
            ind.col = 0;
            ++ind.row;
        }

        attr1PlaceableToEmpty -= attr1Count;
        attr2PlaceableToEmpty -= attr2Count;

        int maxAttrPresence = attr1Count;
        int minAttrPresence = attr2Count;
        if (minAttrPresence > maxAttrPresence) {
            std::swap(maxAttrPresence, minAttrPresence);
        }

        puocResultState.totalEarnedPoints =
            minAttrPresence * ctx->get_satisfaction_value();

        attr1Count -= minAttrPresence;
        attr2Count -= minAttrPresence;

        /**
         * Adding the completable subpatterns that need only 1 die, because the
         * other attributed die has occured more than the current one
         */
        int onePlaceable = std::min(attr1Count, attr2PlaceableToEmpty) +
                           std::min(attr2Count, attr1PlaceableToEmpty);
        if (onePlaceable > 0) {
            puocResultState.completableSubPatterns.insert(
                puocResultState.completableSubPatterns.end(), onePlaceable,
                PuocPatternState::CompletableSubPattern {1});
        }

        /** Adding the remaining places where both the dice are needed */
        int remainingMinpresent =
            std::min(attr1PlaceableToEmpty, attr2PlaceableToEmpty);
        if (remainingMinpresent > 0) {
            puocResultState.completableSubPatterns.insert(
                puocResultState.completableSubPatterns.end(),
                remainingMinpresent,
                PuocPatternState::CompletableSubPattern {2});
        }

        return puocResultState;
    }

    virtual std::unique_ptr<PublicObjectiveCard> clone() override
    {
        return std::make_unique<
            TwoAttributeCollectingPublicObjectiveCard<Attribute>>(*this);
    }

  private:
    Attribute attr1;
    Attribute attr2;
};

#endif // TWO_ATTRIBUTE_COLLECTING_PUBLIC_OBJECTIVE_CARD_H