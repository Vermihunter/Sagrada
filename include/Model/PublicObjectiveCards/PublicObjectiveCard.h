#ifndef PUBLIC_OBJECTIVE_CARD_H
#define PUBLIC_OBJECTIVE_CARD_H

#include "Attributes.h"
#include "Board.h"
#include "BoardField.h"
#include "Die.h"
#include "OverallBoardState.h"
#include "PublicObjectiveCardContext.h"
#include "PublicObjectiveCardGameContext.h"
#include "PublicObjectiveCardPatternState.h"
#include "PublicObjectiveCardType.h"
#include "Typedefs.h"

#include <memory>
#include <vector>

class PublicObjectiveCard;

using puoc_t = std::unique_ptr<PublicObjectiveCard>;
using puoc_c = std::vector<puoc_t>;

/**
 * @brief Interface for Public objective cards
 *
 */
struct PublicObjectiveCard {
	virtual ~PublicObjectiveCard() = default;

	PublicObjectiveCard(concrete_puoc_context_t _ctx) : ctx(std::move(_ctx)) {}

	/**
	 * @brief Evaluating function called after the game ended
	 *
	 * Each Public Objective Card has to implement this function which
	 * determines the rules and the score after the game is over.
	 */
	virtual puoc_pattern_t evaluate(OverallBoardState& overallState, board_t& board) = 0;

	/**
	 * @brief Clones the Public objective card
	 *
	 * @return std::unique_ptr<PublicObjectiveCard> Cloned Puoc
	 */
	virtual std::unique_ptr<PublicObjectiveCard> clone() = 0;

	const concrete_puoc_context_t& get_ctx() const { return ctx; }

	puoc_etype get_type() { return etype; }

	/**
	 * @brief Factory method that creates a Puoc by the corresponding
	 * enumeration type
	 *
	 * @param type Type of the Puoc to be created
	 * @param ctx Context that is used by the Puoc
	 * @return puoc_t The newly created Puoc
	 */
	static puoc_t create_by_type(puoc_etype type, concrete_puoc_context_t ctx);

  protected:
	puoc_etype              etype;
	concrete_puoc_context_t ctx;
};

#endif // PUBLIC_OBJECTIVE_CARD_H
