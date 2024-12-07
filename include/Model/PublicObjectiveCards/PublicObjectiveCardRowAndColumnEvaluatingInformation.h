#ifndef PUBLIC_OBJECTIVE_CARD_ROW_AND_COLUMN_EVALUATING_INFORMATION_H
#define PUBLIC_OBJECTIVE_CARD_ROW_AND_COLUMN_EVALUATING_INFORMATION_H

#include <unordered_set>
#include <vector>

/**
 * @brief Unified implementation of Row and Column collecting Public objective
 * card patterns
 *
 * @tparam Attribute
 */
template <typename Attribute>
struct PublicObjectiveCardRowAndColumnEvaluatingInformation
{
    PublicObjectiveCardRowAndColumnEvaluatingInformation(size_t _size)
        : attrIndices(_size, 0)
    {
    }

    std::vector<size_t> attrIndices;
    std::unordered_set<Attribute> emptyFieldRestrictions;
};

#endif // PUBLIC_OBJECTIVE_CARD_ROW_AND_COLUMN_EVALUATING_INFORMATION_H