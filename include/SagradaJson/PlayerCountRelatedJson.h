#ifndef PLAYER_COUNT_RELATED_JSON_H
#define PLAYER_COUNT_RELATED_JSON_H

#include "PlayerCountRelatedGameInformation.h"
#include "SagradaJson.h"

void to_json(json& j, const PlayerCountRelatedGameInformation& info)
{
    j = json {{"PlayerCount", info.playerCount}, {"DiceCount", info.diceCount}};
}

#endif // PLAYER_COUNT_RELATED_JSON_H