#ifndef SCORE_CONTEXT_JSON_H
#define SCORE_CONTEXT_JSON_H

#include "SagradaJson.h"
#include "ScoreContext.h"

void to_json(json& j, const score_ctx_t& scoreCtx)
{
    j = json {
        {"pointsPerUnusedFavorToken", scoreCtx->pointsPerUnusedFavorToken},
        {"minusPointsPerEmptyField", scoreCtx->minusPointsPerEmptyField},
        {"pointsPerProcValue", scoreCtx->pointsPerProcValue}};
}

#endif // SCORE_CONTEXT_JSON_H