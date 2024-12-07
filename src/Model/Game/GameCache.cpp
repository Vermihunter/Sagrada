#include "GameCache.h"

#include "GameContextBuilder.h"

cached_game_ctx_builder_t GameCache::get_context(size_t playerCount,
                                                 const std::string& ctxName)
{
    cached_game_ctx_builder_t res;

    auto contextIt = definedContexts.find(playerCount);
    if (contextIt == definedContexts.end()) {
        return nullptr;
    }

    for (auto&& ctx : contextIt->second) {
        if (ctx->get_name() == ctxName) {
            res = ctx;
            break;
        }
    }

    return res;
}
