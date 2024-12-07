#ifndef UNIQUE_COLOR_COLUMN_PUBLIC_OBJECTIVE_CARD_H
#define UNIQUE_COLOR_COLUMN_PUBLIC_OBJECTIVE_CARD_H

#include <vector>

#include "PublicObjectiveCard.h"
#include "PublicObjectiveCardEvaluatingPatternState.h"
#include "PublicObjectiveCardRowAndColumnEvaluatingInformation.h"

/**
 * @brief Implementation of Column Shade Variety and Column Color Variety Public
 * objective cards
 *
 * @tparam Attribute color_t or value_t according to the type of the Public
 * objective card
 */
template <typename Attribute>
class UniqueAttributeColumnPublicObjectiveCard : public PublicObjectiveCard
{
    using col_info_t =
        PublicObjectiveCardRowAndColumnEvaluatingInformation<Attribute>;

  public:
    UniqueAttributeColumnPublicObjectiveCard(concrete_puoc_context_t ctx)
        : PublicObjectiveCard(std::move(ctx))
    {
    }

    UniqueAttributeColumnPublicObjectiveCard(
        const UniqueAttributeColumnPublicObjectiveCard& other)
        : PublicObjectiveCard(other.ctx->clone())
    {
    }

    virtual puoc_pattern_t evaluate(OverallBoardState& overallState,
                                    board_t& board) override
    {
        puoc_pattern_t puocResultState {*this};
        puocResultState.dicePerPattern = board->get_ctx()->config->height;

        std::vector<col_info_t> attributeCountByColumn(
            board->get_ctx()->config->width, // one for each column
            size_t(Attribute::COUNT) + 1 // fill them with 0 for each attribute
        );

        BoardIndex ind {0, 0};
        for (auto&& row : board->get_ctx()->data) {
            for (auto&& field : row) {
                Die* die = field->get_die();
                if (!die) {
                    auto fieldRestriciton =
                        field->template get_attr_restriction<Attribute>();
                    // Remembering restrictions for pattern completability
                    // checking
                    if (fieldRestriciton.has_value()) {
                        attributeCountByColumn[ind.col]
                            .emptyFieldRestrictions.insert(
                                fieldRestriciton.value());
                    }
                    // Incrementing the last element that represents empty
                    // fields
                    attributeCountByColumn[ind.col].attrIndices.back() += 1;

                    ++ind.col;
                    continue;
                }

                size_t attributeInd = size_t(Die::get_attr<Attribute>(die));
                attributeCountByColumn[ind.col].attrIndices[attributeInd] += 1;

                ++ind.col;
            }
            ind.col = 0;
            ++ind.row;
        }

        for (auto&& columnAttributes : attributeCountByColumn) {
            puoc_eval_pattern_state state = column_pattern_match(
                columnAttributes, board->get_ctx()->config);

            if (state == puoc_eval_pattern_state::MATCHED) {
                puocResultState.totalEarnedPoints +=
                    ctx->get_satisfaction_value();
            }
            else if (state == puoc_eval_pattern_state::CANNOT_BE_MATCHED) {
                ++puocResultState.uncompletableSubPatternCount;
            }
            else {
                int diceToComplete = dice_to_complete_column_pattern(
                    overallState, columnAttributes, board->get_ctx()->config);
                if (diceToComplete > 0) {
                    puocResultState.completableSubPatterns.push_back(
                        {diceToComplete});
                }
                else {
                    puocResultState.uncompletableSubPatternCount += 1;
                }
            }
        }

        return puocResultState;
    }

    virtual std::unique_ptr<PublicObjectiveCard> clone() override
    {
        return std::make_unique<
            UniqueAttributeColumnPublicObjectiveCard<Attribute>>(*this);
    }

  private:
    int dice_to_complete_column_pattern(OverallBoardState& overallState,
                                        col_info_t& columnAttributes,
                                        const board_config_t& config)
    {
        // The row has no empty fields meaning the pattern is either
        // successfully matched or cannot be matched without using tool cards
        if (columnAttributes.attrIndices.back() == 0) {
            return -1;
        }

        // Checking whether the row contains a die with the same attribute as
        // one of the empty fields has as restriction. This way we can be sure
        // that the pattern cannot be matched
        for (size_t i = 0; i < columnAttributes.attrIndices.size() - 1; ++i) {
            if (columnAttributes.attrIndices[i] == 0) {
                continue;
            }

            Attribute attr = Attribute(i);
            if (columnAttributes.emptyFieldRestrictions.find(attr) !=
                columnAttributes.emptyFieldRestrictions.end()) {
                return -1;
            }
        }

        // If non of the checks evaluated to the state where the pattern
        // cannot be matched, the number of empty fields is returned
        return columnAttributes.attrIndices.back();
    }

    puoc_eval_pattern_state column_pattern_match(col_info_t& columnAttributes,
                                                 const board_config_t& config)
    {
        for (size_t i = 0; i < columnAttributes.attrIndices.size() - 1; ++i) {
            if (columnAttributes.attrIndices[i] >= 2) {
                return puoc_eval_pattern_state::CANNOT_BE_MATCHED;
            }
        }

        /**
         * if there were no attributes present more than once and there is no
         * empty field, that means there were only distinct attributes in the
         * row
         */
        return (columnAttributes.attrIndices.back() == 0)
                   ? puoc_eval_pattern_state::MATCHED
                   : puoc_eval_pattern_state::NONE;
    }
};

#endif // UNIQUE_COLOR_COLUMN_PUBLIC_OBJECTIVE_CARD_H