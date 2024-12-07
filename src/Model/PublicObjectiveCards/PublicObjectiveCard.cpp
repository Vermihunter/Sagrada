#include "PublicObjectiveCard.h"

#include "AllAttributeCollectingPublicObjectiveCard.h"
#include "DiagonalConnectingPublicObjectiveCard.h"
#include "TwoAttributeCollectingPublicObjectiveCard.h"
#include "UniqueAttributeColumnPublicObjectiveCard.h"
#include "UniqueAttributeRowPublicObjectiveCard.h"

puoc_t PublicObjectiveCard::create_by_type(puoc_etype type,
                                           concrete_puoc_context_t ctx)
{
    puoc_t result;
    switch (type) {
        case puoc_etype::UNIQUE_VALUE_ROW:
            result = std::make_unique<
                UniqueAttributeRowPublicObjectiveCard<value_t>>(std::move(ctx));
            break;
        case puoc_etype::UNIQUE_COLOR_ROW:
            result = std::make_unique<
                UniqueAttributeRowPublicObjectiveCard<color_t>>(std::move(ctx));
            break;
        case puoc_etype::UNIQUE_VALUE_COLUMN:
            result = std::make_unique<
                UniqueAttributeColumnPublicObjectiveCard<value_t>>(
                std::move(ctx));
            break;
        case puoc_etype::UNIQUE_COLOR_COLUMN:
            result = std::make_unique<
                UniqueAttributeColumnPublicObjectiveCard<color_t>>(
                std::move(ctx));
            break;
        case puoc_etype::ONE_AND_TWO_COLLECTING:
            result = std::make_unique<
                TwoAttributeCollectingPublicObjectiveCard<value_t>>(
                std::move(ctx), value_t::ONE, value_t::TWO);
            break;
        case puoc_etype::THREE_AND_FOUR_COLLECTING:
            result = std::make_unique<
                TwoAttributeCollectingPublicObjectiveCard<value_t>>(
                std::move(ctx), value_t::THREE, value_t::FOUR);
            break;
        case puoc_etype::FIVE_AND_SIX_COLLECTING:
            result = std::make_unique<
                TwoAttributeCollectingPublicObjectiveCard<value_t>>(
                std::move(ctx), value_t::FIVE, value_t::SIX);
            break;
        case puoc_etype::ALL_VALUE_COLLECTING:
            result = std::make_unique<
                AllAttributeCollectingPublicObjectiveCard<value_t>>(
                std::move(ctx));
            break;
        case puoc_etype::ALL_COLOR_COLLECTING:
            result = std::make_unique<
                AllAttributeCollectingPublicObjectiveCard<color_t>>(
                std::move(ctx));
            break;
        case puoc_etype::DIAGONAL_CONNECTING:
            result = std::make_unique<
                DiagonalConnectingPublicObjectiveCard<color_t>>(std::move(ctx));
            break;

        case puoc_etype::COUNT:
        default:
            throw std::invalid_argument {
                "Invalid enum Public objective card enum type!"};
    }

    result->etype = type;
    return result;
}