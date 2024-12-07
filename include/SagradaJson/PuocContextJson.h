#ifndef PUOC_CONTEXT_JSON_H
#define PUOC_CONTEXT_JSON_H

#include "PublicObjectiveCardGameContext.h"
#include "SagradaJson.h"

void to_json(
    json& j,
    const std::pair<puoc_etype, PublicObjectiveCardContext>& concretePuocCtx)
{
    j = json {
        {"PuocId", concretePuocCtx.first},
        {"SatisfactionValue", concretePuocCtx.second.get_satisfaction_value()}};
}

void to_json(json& j, const puoc_context_t& puocCtx)
{
    j = json {{"puoc_contexts", puocCtx->get_current_game_puocs()},
              {"cards_per_game", puocCtx->cards_per_game()}};
}

#endif // PUOC_CONTEXT_JSON_H