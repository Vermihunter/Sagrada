#ifndef WPC_JSON_H
#define WPC_JSON_H

#include "BoardContext.h"
#include "SagradaJson.h"
#include "Typedefs.h"

void to_json(json& j, const wpc_t& tcCtx) { j = tcCtx->title; }

#endif // WPC_JSON_H