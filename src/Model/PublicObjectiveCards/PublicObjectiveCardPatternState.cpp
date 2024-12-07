#include "PublicObjectiveCardPatternState.h"

#include "PublicObjectiveCard.h"

PuocPatternState::PuocPatternState(const PublicObjectiveCard& _puoc)
    : puoc(_puoc), totalEarnedPoints(0), uncompletableSubPatternCount(0)
{
}

PuocPatternState::~PuocPatternState() {}