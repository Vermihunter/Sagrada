#ifndef I_TOOL_CARD_VIEW_SIGNAL_PROCESSOR_H
#define I_TOOL_CARD_VIEW_SIGNAL_PROCESSOR_H

#include "DieToFieldMoveMode.h"
#include "MoveResultHandlingPolicy.h"
#include "Typedefs.h"

class ToolCardView;
class BlockedUserInterraction;

/**
 * @brief Interface for different game playing page's to communicate with Tool
 * card views
 */
struct IToolCardViewSignalProcessor {
	virtual ~IToolCardViewSignalProcessor()                                               = default;
	virtual void tool_card_clicked(ToolCardView*, bool forceSelect = false)               = 0;
	virtual bool send_move_request(move_t move, const MoveResultHandlingPolicy&)          = 0;
	virtual void show_error_message(const std::string& msg)                               = 0;
	virtual void highlight_basic_moves_for_die(Die* die, ID_t playerID,
	                                           dtfm_mode_mask_t mode = DTFM_MODE_DEFAULT) = 0;
	virtual void highlight_fields(const board_ind_c& indices, ID_t playerID)              = 0;
	virtual void redraw_curr_rounds_dice()                                                = 0;
	virtual void block_features(const BlockedUserInterraction& newState)                  = 0;
	virtual void restore_default_blocking_state()                                         = 0;
};

#endif // I_TOOL_CARD_VIEW_SIGNAL_PROCESSOR_H