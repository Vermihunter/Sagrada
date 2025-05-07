#include "RelocatingTC.h"

#include "Game.h"
#include "GameInfo.h"

const std::vector<std::string> RelocatingTC::indexErrorStringTable = {
    "", // No error
    "Cannot place two dice to the same field!", "Cannot move the same die to two fields!",
    "You have to relocate two different dice!"};

const std::vector<std::string> RelocatingTC::moveErrorStringTable = {
    "", // No error
    "Cannot put a die\nto the same place!", "The field doesn't\ncontain a die to move!",
    "Basic move incorrect!"};

void RelocatingTC::undo_relocate_move(relocate_move_t relocMove, Game& g)
{
	auto& currPlayerBoard = *GameInfo::get_player_with_id(g, relocMove->playerId)->board;
	Die*  relocatedDie    = currPlayerBoard[relocMove->to]->remove_die();
	currPlayerBoard[relocMove->from]->put_die(relocatedDie);
}

void RelocatingTC::use_relocate_move(relocate_move_t relocMove, Game& g)
{
	auto  playerID        = relocMove->playerId;
	auto& currPlayerBoard = *GameInfo::get_player_with_id(g, playerID)->board;

	board_field_t& boardField    = currPlayerBoard[relocMove->from];
	Die*           dieToRelocate = boardField->remove_die();

	DieToFieldMove dtfm{playerID, dieToRelocate, relocMove->to, relocMove->mode};
	try {
		g.move_request(&dtfm, true);
	}
	// In case the move was not correct, we put back the die
	catch (BadMoveRequest& bmr) {
		boardField->put_die(dieToRelocate);
		throw bmr;
	}
}

void RelocatingTC::use_two_relocate_moves(relocate_two_dice_move_t mv, Game& g)
{
	use_relocate_move(&mv->m1, g);

	// At this point the first relocate move was successful, but the second may
	// fail, that way the first has to be undone
	try {
		use_relocate_move(&mv->m2, g);
	}
	catch (BadMoveRequest& bmr) {
		undo_relocate_move(&mv->m1, g);
		throw bmr;
	}
}

relocating_check_res_t
RelocatingTC::check_relocate_move_correct(const BoardIndex& from, const BoardIndex& to,
                                          ID_t playerId, dtfm_mode_mask_t relocMoveMode, Game& g)
{
	if (from == to) {
		return relocating_check_res_t::TO_EQ_FROM;
	}

	auto  playerID        = playerId;
	auto& currPlayerBoard = GameInfo::get_player_with_id(g, playerID)->board;

	auto& fromField     = (*currPlayerBoard)[from];
	Die*  dieToRelocate = fromField->get_die();

	if (!dieToRelocate) {
		return relocating_check_res_t::NO_DIE;
	}

	DieToFieldMove bm{playerID, dieToRelocate, to, relocMoveMode};
	fromField->remove_die();

	if (!g.get_ctx()->basicMoveHandler->check_move_correct_softfail(g, &bm, true)) {
		fromField->put_die(dieToRelocate);
		return relocating_check_res_t::DTFM_BAD;
	}

	fromField->put_die(dieToRelocate);
	return relocating_check_res_t::MOVE_OK;
}

bool RelocatingTC::check_relocate_move_correct_softfail(const BoardIndex& from,
                                                        const BoardIndex& to, ID_t playerId,
                                                        dtfm_mode_mask_t relocMoveMode, Game& g)
{
	return check_relocate_move_correct(from, to, playerId, relocMoveMode, g) ==
	       relocating_check_res_t::MOVE_OK;
}

void RelocatingTC::check_relocate_move_correct_hardfail(relocate_move_t relocMove, Game& g)
{
	relocating_check_res_t res = check_relocate_move_correct(
	    relocMove->from, relocMove->to, relocMove->playerId, relocMove->mode, g);
	if (res != relocating_check_res_t::MOVE_OK) {
		LOG_D("BadMoveRequest 22");
		throw BadMoveRequest{moveErrorStringTable[size_t(res)]};
	}
}

void RelocatingTC::append_two_relocate_move_if_correct(ToolCardAppendContext& ctx,
                                                       const RelocateDieMove& m1,
                                                       const RelocateDieMove& m2)
{
	auto& board = *GameInfo::get_player_with_id(ctx.g, m1.playerId)->board;
	// The indices must be checked separately because this way no relocate move
	// has to be undone
	if (!check_indices_correct_softfail(m1, m2)) {
		return;
	}

	auto& move2FromField = board[m2.from];
	Die*  move2Die       = move2FromField->remove_die();

	// Both the relocate moves are correct one-by-one, but this checks for
	// additional requirements e.g. this avoids placing two dice with same
	// attributes to neighboring fields
	DieToFieldMove secondRelocateMove{ctx.playerId, move2Die, m2.to, DTFM_MODE_DEFAULT};
	// We are not actually sending a move request, because its has no additional
	// effect
	if (ctx.g.get_ctx()->basicMoveHandler->check_move_correct_softfail(ctx.g, &secondRelocateMove,
	                                                                   true)) {
		ctx.moveContainer.push_back(new RelocateTwoDiceMove(m1, m2));
	}

	move2FromField->put_die(move2Die);
}

bool RelocatingTC::check_indices_correct_softfail(const RelocateDieMove& mv1,
                                                  const RelocateDieMove& mv2)
{
	return check_indices_correct(mv1, mv2) == relocating_index_check_res_t::IND_OK;
}

void RelocatingTC::check_indices_correct_hardfail(const RelocateDieMove& mv1,
                                                  const RelocateDieMove& mv2)
{
	relocating_index_check_res_t res = check_indices_correct(mv1, mv2);
	if (res != relocating_index_check_res_t::IND_OK) {
		LOG_D("BadMoveRequest 21");
		throw BadMoveRequest{indexErrorStringTable[size_t(res)]};
	}
}

relocating_index_check_res_t RelocatingTC::check_indices_correct(const RelocateDieMove& m1,
                                                                 const RelocateDieMove& m2)
{
	if (m1.to == m2.to) {
		return relocating_index_check_res_t::TWO_SAME_TO;
	}

	if (m1.from == m2.from) {
		return relocating_index_check_res_t::TWO_SAME_FROM;
	}

	if (m2.from == m1.to) {
		return relocating_index_check_res_t::TO_EQ_FROM;
	}

	return relocating_index_check_res_t::IND_OK;
}

/*
template<typename T, typename ContainerFiller>
void RelocatingTC::append_all_relocate_moves(
    std::vector<T>& destContainer, ToolCardAppendContext& ctx, dtfm_mode_mask_t
mode
)
*/
