#ifndef ROUND_INFO_JSON_H
#define ROUND_INFO_JSON_H

#include "GameHistory.h"
#include "SagradaJson.h"

void to_json(json& j, const TurnInfo& turnInfo)
{
    j = json {{"PlayerId", turnInfo.playerId},
              {"ToolCardUsed", turnInfo.tcUsed}};
}

void to_json(json& j, const round_info_t& roundInfo)
{
    j = json {{"TurnInfos", roundInfo->turnInfos}};
}

#endif // ROUND_INFO_JSON_H