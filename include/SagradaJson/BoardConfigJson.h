#ifndef BOARD_CONFIG_JSON_H
#define BOARD_CONFIG_JSON_H

#include "BoardConfig.h"
#include "SagradaJson.h"
#include "Typedefs.h"

void to_json(json& j, const board_config_t& boardConfig)
{
    j = json {{"BoardType", boardConfig->type->get_name()},
              {"Width", boardConfig->width},
              {"Height", boardConfig->height}};
}

#endif // BOARD_CONFIG_JSON_H