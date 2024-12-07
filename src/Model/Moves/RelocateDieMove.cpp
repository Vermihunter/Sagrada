#include "RelocateDieMove.h"

#include "Game.h"
#include "GameInfo.h"

Die* RelocateDieMove::die_on_from_field(Game& g)
{
    auto& currPlayerBoard = GameInfo::player_on_move(g)->board;

    // auto& [rowInd, colInd] = from;
    auto& fromField = (*currPlayerBoard)[from];
    return fromField->get_die();
}
