#ifndef UNIQUE_VALUE_ROW_PUBLIC_OBJECTIVE_CARD_H
#define UNIQUE_VALUE_ROW_PUBLIC_OBJECTIVE_CARD_H

#include <vector>

#include "PublicObjectiveCard.h"
#include "PublicObjectiveCardEvaluatingPatternState.h"
#include "PublicObjectiveCardRowAndColumnEvaluatingInformation.h"

/**
 * @brief Implementation of Row Shade Variety and Row Color Variety Public
 * objective cards
 *
 * @tparam Attribute color_t or value_t according to the type of the Public
 * objective card
 */
template <typename Attribute>
class UniqueAttributeRowPublicObjectiveCard : public PublicObjectiveCard
{
  public:
    using row_info_t =
        PublicObjectiveCardRowAndColumnEvaluatingInformation<Attribute>;

    UniqueAttributeRowPublicObjectiveCard(concrete_puoc_context_t ctx)
        : PublicObjectiveCard(std::move(ctx))
    {
    }

    UniqueAttributeRowPublicObjectiveCard(
        const UniqueAttributeRowPublicObjectiveCard& other)
        : PublicObjectiveCard(other.ctx->clone())
    {
    }

    virtual puoc_pattern_t evaluate(OverallBoardState& overallState,
                                    board_t& board) override
    {
        puoc_pattern_t puocResultState {*this};
        puocResultState.dicePerPattern = board->get_ctx()->config->width;

        std::vector<bool> unmatchableRows(board->get_ctx()->config->height,
                                          false);
        for (auto&& unfillableFieldInd : overallState.unfillableFieldIndices) {
            unmatchableRows[unfillableFieldInd.row] = true;
        }

        std::vector<row_info_t> attributeCountByRow(
            board->get_ctx()->config->height, // one for each Row
            size_t(Attribute::COUNT) + 1      // adding extra for empty field
        );

        BoardIndex ind {0, 0};
        for (auto&& row : board->get_ctx()->data) {
            // If we know that a row is unmatchable, there is no reason to
            // analyze it
            if (unmatchableRows[ind.row]) {
                ++ind.row;
                continue;
            }

            for (auto&& field : row) {
                Die* die = field->get_die();
                if (!die) {
                    auto fieldRestriciton =
                        field->template get_attr_restriction<Attribute>();
                    // Remembering restrictions for pattern completability
                    // checking
                    if (fieldRestriciton.has_value()) {
                        attributeCountByRow[ind.row]
                            .emptyFieldRestrictions.insert(
                                fieldRestriciton.value());
                    }
                    // Incrementing the last element that represents empty
                    // fields
                    attributeCountByRow[ind.row].attrIndices.back() += 1;
                    ++ind.col;
                    continue;
                }

                size_t attributeInd = size_t(Die::get_attr<Attribute>(die));
                attributeCountByRow[ind.row].attrIndices[attributeInd] += 1;
                ++ind.col;
            }
            ind.col = 0;
            ++ind.row;
        }

        size_t rowInd = 0;
        for (auto&& rowAttributes : attributeCountByRow) {
            if (unmatchableRows[rowInd]) {
                ++puocResultState.uncompletableSubPatternCount;
                continue;
            }

            puoc_eval_pattern_state state =
                row_pattern_match(rowAttributes, board->get_ctx()->config);

            // Pattern match
            if (state == puoc_eval_pattern_state::MATCHED) {
                puocResultState.totalEarnedPoints +=
                    ctx->get_satisfaction_value();
            }
            // The pattern was not matched and from the previous search it may
            // or may not be completed
            else if (state != puoc_eval_pattern_state::CANNOT_BE_MATCHED) {
                int diceToComplete = dice_to_complete_row_pattern(
                    overallState, rowAttributes, board->get_ctx()->config);
                if (diceToComplete > 0) {
                    puocResultState.completableSubPatterns.push_back(
                        {diceToComplete});
                }
                else {
                    puocResultState.uncompletableSubPatternCount += 1;
                }
            }
            else {
                puocResultState.uncompletableSubPatternCount += 1;
            }

            ++rowInd;
        }

        return puocResultState;
    }

    virtual std::unique_ptr<PublicObjectiveCard> clone() override
    {
        return std::make_unique<
            UniqueAttributeRowPublicObjectiveCard<Attribute>>(*this);
    }

  private:
    puoc_eval_pattern_state row_pattern_match(row_info_t& rowAttributes,
                                              const board_config_t& config)
    {
        for (size_t i = 0; i < rowAttributes.attrIndices.size() - 1; ++i) {
            if (rowAttributes.attrIndices[i] >= 2) {
                return puoc_eval_pattern_state::CANNOT_BE_MATCHED;
            }
        }

        /**
         * if there were no attributes present more than once and there is no
         * empty field, that means there were only distinct attributes in the
         * row
         */
        return (rowAttributes.attrIndices.back() == 0)
                   ? puoc_eval_pattern_state::MATCHED
                   : puoc_eval_pattern_state::NONE;
    }

    int dice_to_complete_row_pattern(OverallBoardState& overallState,
                                     row_info_t& rowAttributes,
                                     const board_config_t& config)
    {
        // The row has no empty fields meaning the pattern is either
        // successfully matched or cannot be matched without using tool cards
        if (rowAttributes.attrIndices.back() == 0) {
            return -1;
        }

        // Checking whether the row contains a die with the same attribute as
        // one of the empty fields has as restriction. This way we can be sure
        // that the pattern cannot be matched
        for (size_t i = 0; i < rowAttributes.attrIndices.size() - 1; ++i) {
            if (rowAttributes.attrIndices[i] == 0) {
                continue;
            }

            Attribute attr = Attribute(i);
            if (rowAttributes.emptyFieldRestrictions.find(attr) !=
                rowAttributes.emptyFieldRestrictions.end()) {
                return -1;
            }
        }

        // If non of the checks evaluated to the state where the pattern
        // cannot be matched, the number of empty fields is returned
        return rowAttributes.attrIndices.back();
    }
};

#endif // UNIQUE_VALUE_ROW_PUBLIC_OBJECTIVE_CARD_H