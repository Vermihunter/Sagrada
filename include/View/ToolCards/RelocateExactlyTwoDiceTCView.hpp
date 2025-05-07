#ifndef RELOCATEEXACTLYTWODICETCVIEW_HPP
#define RELOCATEEXACTLYTWODICETCVIEW_HPP

#include "RelocateExactlyTwoDiceTC.h"
#include "RelocatingToolCardView.hpp"

class RelocateExactlyTwoDiceTCView : public RelocatingToolCardView {
  public:
	RelocateExactlyTwoDiceTCView(ToolCardViewContainer& _parentContainer, Game& game, ToolCard& _tc,
	                             const SizeInPixels& size, size_t ind)
	    : RelocatingToolCardView(_parentContainer, game, _tc, size, ind)
	{
	}
};

#endif // RELOCATEEXACTLYTWODICETCVIEW_HPP