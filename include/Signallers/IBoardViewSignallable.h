#ifndef I_BOARD_VIEW_SIGNALLABLE_H
#define I_BOARD_VIEW_SIGNALLABLE_H

#include "BoardIndex.h"

class RectangularBoardView;

/**
 * @brief Represents an interface that the BoardView sees through the concrete
 * signallables Defined for the BoardView's observer implementation
 */
struct IBoardViewSignallable
{
    virtual ~IBoardViewSignallable() = default;
    virtual void receive_wpc_click_signal(RectangularBoardView*) {}
    virtual void receive_board_field_click_signal(RectangularBoardView*,
                                                  const BoardIndex& ind)
    {
    }
};

#endif // I_BOARD_VIEW_SIGNALLABLE_H