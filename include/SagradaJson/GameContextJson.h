#ifndef GAME_CONTEXT_TO_JSON_H
#define GAME_CONTEXT_TO_JSON_H

#include "BoardConfigJson.h"
#include "DiceConfigJson.h"
#include "GameContextBuilder.h"
#include "PlayerCountRelatedJson.h"
#include "PuocContextJson.h"
#include "SagradaJson.h"
#include "ScoreContextJson.h"
#include "TcContextJson.h"
#include "WpcJson.h"

namespace nlohmann {
template <> struct adl_serializer<GameContextBuilder> {
	static void to_json(json& j, const GameContextBuilder& builder)
	{
		j = json{{"PuocConfig", builder.puocContext},
		         {"TcConfig", builder.tcContext},
		         {"PlayerCountRelatedInformation", builder.playerCountRelatedConfigs},
		         {"ScoreContext", builder.ctx->scoreCtx},
		         {"DiceConfig", builder.diceConfig},
		         {"wpcChoicePerPlayer", builder.wpcChoicePerPlayer.value()},
		         {"NumberOfRounds", builder.numberOfRounds.value()},
		         {"SelectableWPC", builder.ctx->selectableWPC},
		         {"BoardConfig", builder.boardConfig}

		};
	}
};

} // namespace nlohmann

#endif // GAME_CONTEXT_TO_JSON_H