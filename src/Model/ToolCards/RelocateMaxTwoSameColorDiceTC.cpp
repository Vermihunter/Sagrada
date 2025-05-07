#include "RelocateMaxTwoSameColorDiceTC.h"

#include "Game.h"
#include "RelocateDieMoveReferenceFiller.h"

RelocateMaxTwoSameColorDiceTC::RelocateMaxTwoSameColorDiceTC()
{
	// ctor
}

ToolCardUseFeedback RelocateMaxTwoSameColorDiceTC::use(tool_card_move_t m, Game& g)
{
	auto  mv              = to_concrete_type<RelocateTwoDiceMove>(m);
	auto& currPlayerBoard = GameInfo::get_player_with_id(g, mv->playerId)->board;

	// Ensures that there is a Die on the field as well
	check_relocate_move_correct_hardfail(&mv->m1, g);
	Die* dieToRelocate1 = (*currPlayerBoard)[mv->m1.from]->get_die();
	// Game rule
	if (!round_track_contains_color(g, dieToRelocate1->get_color())) {
		LOG_D("BadMoveRequest 18");
		throw BadMoveRequest{"There must be a die with same color on the "
		                     "round track as the moved die"};
	}

	if (mv->hasSecondRelocateMove) {
		check_relocate_move_correct_hardfail(&mv->m2, g);

		Die* fromFieldDie1 = mv->m1.die_on_from_field(g);
		Die* fromFieldDie2 = mv->m2.die_on_from_field(g);

		if (fromFieldDie1->get_color() != fromFieldDie2->get_color()) {
			LOG_D("BadMoveRequest 19");
			throw BadMoveRequest{"The two relocate dice\nmust have the same color!"};
		}

		use_two_relocate_moves(mv, g);
	}
	else {
		use_relocate_move(&mv->m1, g);
	}

	base_use_impl();
	return ToolCardUseFeedback{.shiftRound = false, .tcUsageFinished = true, .extractCost = true};
}

void RelocateMaxTwoSameColorDiceTC::append_possible_moves(ToolCardAppendContext& ctx)
{
	std::array<bool, Color::SIZE> roundTrackContainsColors;
	for (size_t i = 0; i < Color::SIZE; ++i) {
		roundTrackContainsColors[i] = round_track_contains_color(ctx.g, color_t(i));
	}

	std::vector<RelocateDieMove> relocMoves;
	append_all_relocate_moves<RelocateDieMove, RelocateDieMoveReferenceFiller>(relocMoves, ctx);

	DieToFieldMove dummyDtfm{ctx.playerId, nullptr, BoardIndex{}, DTFM_MODE_DEFAULT};
	auto&          board = *GameInfo::get_player_with_id(ctx.g, ctx.playerId)->board;
	for (auto&& relocMove1 : relocMoves) {
		// The get_all_relocate_moves() already checks this, so this is
		// certainly possible
		board_field_t& move1FromField = board[relocMove1.from];
		Die*           move1Die       = move1FromField->remove_die();
		if (!roundTrackContainsColors[size_t(move1Die->get_color())]) {
			move1FromField->put_die(move1Die);
			continue;
		}

		ctx.moveContainer.push_back(new RelocateTwoDiceMove(relocMove1));

		dummyDtfm.die          = move1Die;
		dummyDtfm.boardIndices = relocMove1.to;
		ctx.g.move_request(&dummyDtfm, true);

		for (auto&& relocMove2 : relocMoves) {
			Die* move2Die = board[relocMove2.from]->get_die();
			// The only case is when relocMove2->from == relocMove1->from
			if (!move2Die) {
				continue;
			}

			// If its the same die or they have different colors, its not
			// appended
			if (move1Die->get_color() != move2Die->get_color()) {
				continue;
			}

			append_two_relocate_move_if_correct(ctx, relocMove1, relocMove2);
		}

		undo_relocate_move(&relocMove1, ctx.g);
	}
}

bool RelocateMaxTwoSameColorDiceTC::round_track_contains_color(Game& g, color_t color)
{
	return g.get_ctx()->roundTrack.contains_die_with_color(color);
}

ToolCardUseFeedback RelocateMaxTwoSameColorDiceTC::undo(tool_card_move_t& m, Game& g)
{
	auto relocMoves = to_concrete_type<RelocateTwoDiceMove>(m);

	undo_relocate_move(&relocMoves->m1, g);

	if (relocMoves->hasSecondRelocateMove) {
		undo_relocate_move(&relocMoves->m2, g);
	}
	base_undo_impl();
	return ToolCardUseFeedback{.shiftRound = false, .tcUsageFinished = true, .extractCost = true};
}

RelocateMaxTwoSameColorDiceTC::~RelocateMaxTwoSameColorDiceTC()
{
	// dtor
}
