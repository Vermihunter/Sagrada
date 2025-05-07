#ifndef TC_CONTEXT_JSON_H
#define TC_CONTEXT_JSON_H

#include "SagradaJson.h"
#include "ToolCardGameContext.h"

void to_json(json& j, const tc_context_t& tcCtx)
{
	j = json{{"selectable_ids", tcCtx->get_current_game_tcs()},
	         {"cards_per_game", tcCtx->cards_per_game()}};
}

#endif // TC_CONTEXT_JSON_H