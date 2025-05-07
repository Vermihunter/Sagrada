#ifndef BASIC_MOVE_HANDLER_H
#define BASIC_MOVE_HANDLER_H

#include "BoardConfig.h"
#include "DieToFieldMove.h"
#include "DieToFieldMoveMode.h"

#include <memory>
#include <vector>

class Game;

/**
 * @brief Enumerated errors that can occur during a basic move correctness check
 */
enum class basic_move_error_t {
	NO_ERROR,
	UNDEFINED_BASIC_MOVE,
	FIRST_MOVE_BAD_POS,
	DIE_NULLPTR,
	DIE_NOT_FROM_CURRENTROUND,
	VALUE_COLLIDES_WITH_NEIGHBOR,
	COLOR_COLLIDES_WITH_NEIGHBOR,
	FIELD_UNREACHABLE,
	FIELD_HAS_DIE,
	VALUE_RESTRICTION_ERROR,
	COLOR_RESTRICTION_ERROR,
};

/**
 * @brief Separated basic move handler from the Game object for better
 * readability reasons
 *
 * The check_move_correct() function contains a hardfail and a softfail version
 * for performance reasons.
 *
 * The hardfail version is used only when the User really needs to know the
 * string representation of the error.
 *
 * On the other hand, the softfail version is way faster and is used anytime
 * when the caller doesn't want to know the exact problem that occured and is
 * only interested in whether the move is correct or not.
 */
class BasicMoveHandler {
	/**
	 * @brief String table for the errors that could occur during the
	 * correctness check It is indexed by the basic_move_error_t constants i.e.
	 * the error message for error code e could be found at
	 * errorStringTable[int(e)]
	 */
	const static std::vector<std::string> errorStringTable;

  public:
	virtual ~BasicMoveHandler() = default;
	BasicMoveHandler(const BoardConfig* boardConfig);

	auto clone() { return std::make_unique<BasicMoveHandler>(*this); }

	/**
	 * @brief Throws an exception with the appropriate message if the move is
	 * not correct User friendly reason
	 * @param g Game object that the move is defined on
	 * @param move The move to check for correctness
	 * @param allowAnyDie True if the basic move is part of a Tool card move
	 */
	void check_move_correct_hardfail(Game& g, const die_to_field_move_t& move,
	                                 bool allowAnyDie = false);

	/**
	 * @brief Doesn't throw exception but returns the correctness of a move
	 * Performance reason
	 * @param g Game object that the move is defined on
	 * @param move The move to check for correctness
	 * @param allowAnyDie True if the basic move is part of a Tool card move
	 * @return true If the move was correct
	 * @return false If the move is invalid
	 */
	bool check_move_correct_softfail(Game& g, const die_to_field_move_t& move,
	                                 bool allowAnyDie = false);

	/**
	 * @brief Undo operation for a basic move
	 * Currently unused
	 *
	 * @param g The game object that has move as the last move
	 * @param move The move to undo
	 * @return Die* Die that has been removed from the board if any, otherwise
	 * nullptr
	 */
	Die* undo(Game& g, const die_to_field_move_t& move);

	/**
	 * @brief Uses a basic move
	 * Doesn't check if the move is correct or not. For that reason a preemptive
	 * call must be made to any of the check_move_correct functions and use this
	 * function only on success.
	 * @param g Game object that will receive a move
	 * @param move Move to be made
	 */
	void use(Game& g, const die_to_field_move_t& move);

	/**
	 * @brief Appends all basic moves for a given player using the selected mode
	 *
	 * @param possibleMoves The destination container
	 * @param game The sender game object
	 * @param playerId The ID of the player that is the moves are generated for
	 * @param _modeOpt The mode of the basic move
	 */
	void append_basic_moves(move_c& possibleMoves, Game& game, ID_t playerId,
	                        dtfm_mode_mask_t _modeOpt = DTFM_MODE_DEFAULT);

	move_t get_first_basic_move(Game& g, ID_t playerId);

	/**
	 * @brief Returns all the indices where a given die could be put in the
	 * selected context
	 *
	 * @param g Game objet which the other parameters are part of
	 * @param playerId ID of the player
	 * @param die Die to be checked for placing
	 * @param mode Mode of the basic move
	 * @return board_ind_c List of board indices where the die could be placed
	 * for the given player and mode
	 */
	board_ind_c indices_to_put_to(Game& g, ID_t playerId, Die* die,
	                              dtfm_mode_mask_t mode = DTFM_MODE_DEFAULT);

	/**
	 * @brief Checks if a given index is reachable on a given board
	 *
	 * @param board The board to be checked
	 * @param ind The index that is checked
	 * @return true If there is at least one direct or diagonal neighbor of the
	 * index containing a die
	 * @return false Otherwise
	 */
	bool is_reachable(Board& board, const BoardIndex& ind);

  protected:
	const BoardConfig* boardConfig;

	/**
	 * @brief Implementation of check_move_correct
	 *
	 * @param g Game object where the context is defined on
	 * @param move Move to be checkced
	 * @param allowAnyDie True if the move is part of a Tool car dmove
	 * @return basic_move_error_t The error code indicating whether the move is
	 * correct or not
	 */
	basic_move_error_t check_move_correct_impl(Game& g, const die_to_field_move_t& move,
	                                           bool allowAnyDie);

	/**
	 * @brief Checks if an index could be used as a die to field move index for
	 * the first move of the player According to the official rules of Sagrada,
	 * the first move has to be made to special indices
	 * @param ind Index to be checked
	 * @return true If the index is a correct field to place the first die on
	 * @return false Otherwise
	 */
	bool is_correct_first_move_ind(const BoardIndex& ind);

	/**
	 * @brief Checks is any of the direct neighbors contain the same attributes
	 * as the new move to be made
	 *
	 * @param g Game object that the context is defined on
	 * @param m Move to be checked
	 * @return basic_move_error_t Error code of the check
	 */
	basic_move_error_t check_neighbor_colliding(Game& g, die_to_field_move_t m);

	/**
	 * @brief Checks if a die is one of the current round's dice
	 * Only used in Die to field moves, the Tool card submoves skip this check
	 * @param g Game object that the context is defined on
	 * @param die Die to be checked
	 * @return basic_move_error_t Error code of the check
	 */
	basic_move_error_t check_die_correct(Game& g, Die* die);

	/**
	 * @brief Implementation of the check_die_correct() function
	 */
	bool die_between_curr_round_dice(Game& g, Die* die);

	/**
	 * @brief Checks if a die fulfils the restrictions that the board field
	 * contains
	 */
	basic_move_error_t check_can_put(board_field_t& field, Die* dieToPut, dtfm_mode_mask_t mode);
};

#endif // BASIC_MOVE_HANDLER_H