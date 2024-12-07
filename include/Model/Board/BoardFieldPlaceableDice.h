#ifndef BOARD_FIELD_PLACEABLE_DICE_H
#define BOARD_FIELD_PLACEABLE_DICE_H

#include "BoardFieldFastPlaceableDice.h"
#include "BoardFieldGeneralPlaceableDice.h"

#if Value::SIZE == 6 && Color::SIZE == 5
using placeable_dice_t = FastPlaceableDice;
#else
using placeable_dice_t = GeneralPlaceableDice;
#endif

#endif // BOARD_FIELD_PLACEABLE_DICE_H