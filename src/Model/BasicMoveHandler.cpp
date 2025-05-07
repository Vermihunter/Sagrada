#include "BasicMoveHandler.h"

#include "BoardFieldNeighborCache.h"
#include "DieToFieldMoveMode.h"
#include "Game.h"

const std::vector<std::string> BasicMoveHandler::errorStringTable = {
    "", // No error
    "Undefined basic move!",
    "First move must be placed non an edge or corner space!",
    "Undefined die found!",
    "Die has to be from the current round's dice!",
    "Invalid move! A neighboring die has the same value!",
    "Invalid move! A neighboring die has the same color!",
    "Unreachable field!The field has to be neighboring to the previously "
    "placed dice!",
    "A die is already placed to this field!",
    "Color error! Cannot place a\ndie with that color there!",
    "Value error! Cannot place a\ndie with that value there!"};

BasicMoveHandler::BasicMoveHandler(const BoardConfig* _boardConfig) : boardConfig(_boardConfig) {}

bool BasicMoveHandler::check_move_correct_softfail(Game& g, const die_to_field_move_t& move,
                                                   bool allowAnyDie)
{
	return check_move_correct_impl(g, move, allowAnyDie) == basic_move_error_t::NO_ERROR;
}

void BasicMoveHandler::check_move_correct_hardfail(Game& g, const die_to_field_move_t& move,
                                                   bool allowAnyDie)
{
	basic_move_error_t err = check_move_correct_impl(g, move, allowAnyDie);
	if (err == basic_move_error_t::NO_ERROR) {
		return;
	}

	LOG_D("BadMoveRequest 1");
	throw BadMoveRequest{errorStringTable[int(err)]};
}

basic_move_error_t BasicMoveHandler::check_move_correct_impl(Game&                      g,
                                                             const die_to_field_move_t& move,
                                                             bool                       allowAnyDie)
{
	die_to_field_move_t dtfm = dynamic_cast<DieToFieldMove*>(move);
	if (!dtfm) {
		return basic_move_error_t::UNDEFINED_BASIC_MOVE;
	}

	if (!allowAnyDie) {
		auto err = check_die_correct(g, dtfm->die);
		if (err != basic_move_error_t::NO_ERROR) {
			return err;
		}
	}

	auto& board      = GameInfo::get_player_with_id(g, dtfm->playerId)->board;
	auto& boardInd   = dtfm->boardIndices;
	auto& boardField = (*board)[boardInd];

	bool isFirstMove = board->is_empty();

	/** Checking whether it is the first non-pass move */
	if (isFirstMove && !is_correct_first_move_ind(boardInd)) {
		return basic_move_error_t::FIRST_MOVE_BAD_POS;
	}

	/** Checking if the requested move would collide with previous moves */
	auto err = check_neighbor_colliding(g, dtfm);
	if (err != basic_move_error_t::NO_ERROR) {
		return err;
	}

	/**
	 * It is not the first move and cannot be reached => none of the neighbors
	 * have a dice on and not having a mode that allows unreachable fields
	 */
	if (!isFirstMove && ((dtfm->mode & DTFM_MODE_ACCEPT_UNREACHABLE) == 0) &&
	    !is_reachable(*board, boardInd)) {
		return basic_move_error_t::FIELD_UNREACHABLE;
	}

	/** Throws exception if the field doesn't fulfill the defined rules */
	return check_can_put(boardField, dtfm->die, dtfm->mode);
}

basic_move_error_t BasicMoveHandler::check_die_correct(Game& g, Die* die)
{
	basic_move_error_t res = basic_move_error_t::NO_ERROR;
	if (die == nullptr) {
		res = basic_move_error_t::DIE_NULLPTR;
	}

	if (!die_between_curr_round_dice(g, die)) {
		res = basic_move_error_t::DIE_NOT_FROM_CURRENTROUND;
	}

	return res;
}

bool BasicMoveHandler::die_between_curr_round_dice(Game& g, Die* die)
{
	auto& currRoundDice = g.get_curr_state()->currentRoundRemainingDice;

	return std::find(currRoundDice.begin(), currRoundDice.end(), die) != currRoundDice.end();
}

Die* BasicMoveHandler::undo(Game& g, const die_to_field_move_t& move)
{
	auto dtfm = dynamic_cast<DieToFieldMove*>(move);
	if (!dtfm) {
		throw BadMoveRequest{"Undefined die to field move!"};
	}

	ID_t playerId = dtfm->playerId;

	return (*g.get_ctx()->players[playerId]->board)[dtfm->boardIndices]->remove_die();
}

void BasicMoveHandler::append_basic_moves(move_c& possibleMoves, Game& game, ID_t playerID,
                                          dtfm_mode_mask_t mode)
{
	const auto& currentRoundRemainingDice = game.get_curr_state()->currentRoundRemainingDice;
	for (auto&& die : currentRoundRemainingDice) {
		for (auto&& ind : indices_to_put_to(game, playerID, die, mode)) {
			possibleMoves.push_back(DieToFieldMove::create(playerID, die, ind, mode));
		}
	}
}

move_t BasicMoveHandler::get_first_basic_move(Game& g, ID_t playerId)
{
	const auto& currentRoundRemainingDice = g.get_curr_state()->currentRoundRemainingDice;
	for (auto&& die : currentRoundRemainingDice) {
		DieToFieldMove putMove{playerId, die, BoardIndex{}, DTFM_MODE_DEFAULT};
		for (auto&& ind : boardConfig->type->get_all_indices()) {
			putMove.boardIndices = ind;
			if (check_move_correct_softfail(g, &putMove, true)) {
				return new DieToFieldMove(putMove);
			}
		}
	}

	return nullptr;
}

bool BasicMoveHandler::is_correct_first_move_ind(const BoardIndex& ind)
{
	return ind.row == 0 || ind.row == boardConfig->height - 1 || // first or last row
	       ind.col == 0 || ind.col == boardConfig->width - 1;    // left or right column
}

basic_move_error_t BasicMoveHandler::check_neighbor_colliding(Game& g, die_to_field_move_t m)
{
	auto& board = g.get_ctx()->players[m->playerId]->board;

	/** Checking if the requested move would collide with previous moves */
	for (auto&& neighborInd :
	     BoardFieldNeighborCache::GetDirectNeighborIndicesFor(m->boardIndices)) {
		auto& nField = (*board)[neighborInd];

		/** Direct neighbor has no die on it */
		if (!nField->get_die()) continue;

		if (nField->get_die()->get_color() == m->die->get_color()) {
			return basic_move_error_t::COLOR_COLLIDES_WITH_NEIGHBOR;
		}
		if (nField->get_die()->get_value() == m->die->get_value()) {
			return basic_move_error_t::VALUE_COLLIDES_WITH_NEIGHBOR;
		}
	}

	return basic_move_error_t::NO_ERROR;
}

bool BasicMoveHandler::is_reachable(Board& board, const BoardIndex& ind)
{
	for (auto&& neighborInd : BoardFieldNeighborCache::GetAllNeighborIndicesFor(ind)) {
		/** If any of the field's neighbor has a die on it, it is marked
		 * reachable */
		if (board[neighborInd]->get_die()) {
			return true;
		}
	}

	return false;
}

void BasicMoveHandler::use(Game& g, const die_to_field_move_t& move)
{
	auto dtfm = dynamic_cast<DieToFieldMove*>(move);
	if (!dtfm) {
		LOG_D("BadMoveRequest 3");
		throw BadMoveRequest{"Undefined basic move!"};
	}

	auto& board = g.get_ctx()->players[move->playerId]->board;
	(*board)[dtfm->boardIndices]->put_die(dtfm->die);
}

board_ind_c BasicMoveHandler::indices_to_put_to(Game& g, ID_t playerId, Die* die,
                                                dtfm_mode_mask_t mode)
{
	board_ind_c res;

	if (die == nullptr) {
		LOG_W("Die was nullptr in BasicMoveHandler::indices_to_put_to");
		return res;
	}

	DieToFieldMove putMove{playerId, die, BoardIndex{}, mode};
	for (auto&& ind : boardConfig->type->get_all_indices()) {
		putMove.boardIndices = ind;
		if (check_move_correct_softfail(g, &putMove, true)) {
			res.push_back(ind);
		}
	}

	return res;
}

/**
 * @brief Checks if a die fulfils the restrictions that the board field contains
 */
basic_move_error_t BasicMoveHandler::check_can_put(board_field_t& field, Die* dieToPut,
                                                   dtfm_mode_mask_t mode)
{
	/** If there is already a die on the board field, cannot put another one to
	 * it */
	if (field->get_die()) {
		return basic_move_error_t::FIELD_HAS_DIE;
	}

	auto& colorRestriction = field->get_color_restriction();
	auto& valueRestriction = field->get_value_restriction();

	bool colorIgnored = (mode & DTFM_MODE_IGNORE_COLOR);
	bool valueIgnored = (mode & DTFM_MODE_IGNORE_VALUE);

	if (!colorIgnored && colorRestriction && dieToPut->get_color() != colorRestriction.value()) {
		return basic_move_error_t::COLOR_RESTRICTION_ERROR;
	}

	if (!valueIgnored && valueRestriction && dieToPut->get_value() != valueRestriction.value()) {
		return basic_move_error_t::VALUE_RESTRICTION_ERROR;
	}

	return basic_move_error_t::NO_ERROR;
}