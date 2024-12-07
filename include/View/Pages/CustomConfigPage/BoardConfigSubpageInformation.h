#ifndef BOARD_CONFIG_SUBPAGE_INFORMATION_H
#define BOARD_CONFIG_SUBPAGE_INFORMATION_H

#include "BoardContext.h"
#include "Typedefs.h"

class BoardConfig;

struct BoardConfigSubpageInformation
{
    BoardConfig* boardConfig;
    wpc_c selectableWpc;
};

#endif // BOARD_CONFIG_SUBPAGE_INFORMATION_H