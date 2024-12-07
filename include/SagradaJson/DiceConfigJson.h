#ifndef DICE_CONFIG_JSON_H
#define DICE_CONFIG_JSON_H

#include <algorithm>
#include <vector>

#include "AttributesJson.h"
#include "DiceConfig.h"
#include "SagradaJson.h"

void to_json(json& j, const dice_config_c& diceCtx)
{
    auto toJsonRepresentation = [&](const auto& configByColor) {
        return json {{"Color", configByColor.first},
                     {"DiceCount", configByColor.second.diceCount}};
    };

    std::vector<json> allConfigs;
    std::transform(diceCtx->diceByColor.cbegin(), diceCtx->diceByColor.cend(),
                   std::back_inserter(allConfigs), toJsonRepresentation);

    j = allConfigs;
}

#endif // DICE_CONFIG_JSON_H