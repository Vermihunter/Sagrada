#ifndef relocate_move_tAX_TWO_SAME_COLOR_DIC_ETC_H
#define relocate_move_tAX_TWO_SAME_COLOR_DIC_ETC_H

#include "Board.h"
#include "RelocateTwoDiceMove.h"
#include "RelocatingTC.h"
#include "Typedefs.h"

/**
 * @brief Implementation of the Tap Wheel Tool card
 *
 * Allows the user to move one or two dice of the same color obeying
 * all placement restrictions. The color of the dice must be present
 * on the round track already.
 */
class RelocateMaxTwoSameColorDiceTC : public RelocatingTC {
  public:
	/** Default constructor */
	RelocateMaxTwoSameColorDiceTC();
	/** Default destructor */
	virtual ~RelocateMaxTwoSameColorDiceTC();

	virtual ToolCardUseFeedback use(tool_card_move_t m, Game& g) override;

	virtual void append_possible_moves(ToolCardAppendContext& ctx) override;

	RelocateMaxTwoSameColorDiceTC(const RelocateMaxTwoSameColorDiceTC& other) : RelocatingTC(other)
	{
	}

	virtual tc_t clone() override { return std::make_unique<RelocateMaxTwoSameColorDiceTC>(*this); }

	virtual ToolCardUseFeedback undo(tool_card_move_t& m, Game& g) override;

  private:
	bool round_track_contains_color(Game& g, color_t color);
};

#endif // relocate_move_tAX_TWO_SAME_COLOR_DIC_ETC_H
