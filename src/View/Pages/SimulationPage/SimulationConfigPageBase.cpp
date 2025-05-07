#include "SimulationConfigPageBase.h"

SimulationConfigPageBase::SimulationConfigPageBase(const ViewContext& ctx,
                                                   const std::string& pageName,
                                                   size_t             localPlayerCount)
    : Page(ctx), pageElements(ctx, pageName, localPlayerCount)
{
	set_size_request(ctx.size.width, ctx.size.height);

	pageElements.button_Start.signal_clicked().connect(
	    sigc::mem_fun(*this, &SimulationConfigPageBase::start_button_clicked));
}
