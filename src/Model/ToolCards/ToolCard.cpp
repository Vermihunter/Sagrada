#include "ToolCard.h"

#include "ChangeDieValueTC.h"
#include "Game.h"
#include "MoveTwiceInARowTC.h"
#include "PutDieWithoutNeighborTC.h"
#include "RelocateDieIgnoreAttributeTC.h"
#include "RelocateExactlyTwoDiceTC.h"
#include "RelocateMaxTwoSameColorDiceTC.h"
#include "ReplaceDieFromSupplyToRoundTrackTC.h"
#include "ReplaceDieWithRandomFromBagTC.h"
#include "RerollAllDiceTC.h"
#include "RerollDieTC.h"
#include "ReverseDieValueTC.h"

void ToolCard::base_use_impl()
{
	++useCount;
}

void ToolCard::base_undo_impl()
{
	--useCount;
}

void ToolCard::append_placing_moves(ToolCardAppendContext& ctx, Die* die)
{
	auto&          moveHandlerCtx = ctx.g.get_ctx()->basicMoveHandler;
	DieToFieldMove bm{ctx.playerId, die, BoardIndex{}, DTFM_MODE_DEFAULT};
	for (auto&& ind : moveHandlerCtx->indices_to_put_to(ctx.g, ctx.playerId, die)) {
		bm.boardIndices = ind;

		ctx.moveContainer.push_back(new PlacingDieMove(ctx.playerId, ctx.toolCardInd, bm));
	}
}

tc_t ToolCard::create(tc_etype type)
{
	tc_t result;
	switch (type) {
	case tc_etype::CHANGE_DIE_VALUE:
		result = std::make_unique<ChangeDieValueTC>();
		break;
	case tc_etype::MOVE_TWICE:
		result = std::make_unique<MoveTwiceInARowTC>();
		break;
	case tc_etype::DIE_WITHOUT_NEIGHBORS:
		result = std::make_unique<PutDieWithoutNeighborTC>();
		break;
	case tc_etype::REROLL_ALL_DICE:
		result = std::make_unique<RerollAllDiceTC>();
		break;
	case tc_etype::RELOCATE_TWO_SAME_COLORED_DICE:
		result = std::make_unique<RelocateMaxTwoSameColorDiceTC>();
		break;
	case tc_etype::REPLACE_FROM_ROUND_TRACK:
		result = std::make_unique<ReplaceDieFromSupplyToRoundTrackTC>();
		break;
	case tc_etype::RANDOM_BAG_DIE:
		result = std::make_unique<ReplaceDieWithRandomFromBagTC>();
		break;
	case tc_etype::REROLL_DIE:
		result = std::make_unique<RerollDieTC>();
		break;
	case tc_etype::REVERSE_DIE_VALUE:
		result = std::make_unique<ReverseDieValueTC>();
		break;
	case tc_etype::MOVE_IGNORE_VAL:
		result = std::make_unique<RelocateDieIgnoreAttributeTC<DTFM_MODE_IGNORE_VALUE>>();
		break;
	case tc_etype::MOVE_IGNORE_COLOR:
		result = std::make_unique<RelocateDieIgnoreAttributeTC<DTFM_MODE_IGNORE_COLOR>>();
		break;
	case tc_etype::RELOCATE_TWO_DICE:
		result = std::make_unique<RelocateExactlyTwoDiceTC>();
		break;
	case tc_etype::COUNT:
		break;
	default:
		break;
	}

	result->etype = type;
	return result;
}