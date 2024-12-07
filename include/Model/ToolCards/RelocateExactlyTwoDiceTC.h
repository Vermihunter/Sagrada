#ifndef RELOCATE_EXACTLY_TWO_DICE_TC_H
#define RELOCATE_EXACTLY_TWO_DICE_TC_H

#include "Board.h"
#include "RelocateTwoDiceMove.h"
#include "RelocatingTC.h"
#include "Typedefs.h"

/**
 * @brief Implementation of the Lathekin Tool card
 *
 * Moves two dice obeying all placement restrictions, reachability and every
 * aspect of a correct die to field move.
 */
class RelocateExactlyTwoDiceTC : public RelocatingTC
{
  public:
    /** Default constructor */
    RelocateExactlyTwoDiceTC();
    /** Default destructor */
    virtual ~RelocateExactlyTwoDiceTC();

    virtual ToolCardUseFeedback use(tool_card_move_t m, Game& g) override;

    virtual void append_possible_moves(ToolCardAppendContext& ctx) override;

    virtual tc_t clone() override
    {
        return std::make_unique<RelocateExactlyTwoDiceTC>(*this);
    }

    virtual ToolCardUseFeedback undo(tool_card_move_t& m, Game& g) override;
};

#endif // RELOCATE_EXACTLY_TWO_DICE_TC_H
