#include "AllSubpages.h"

AllSubpages::AllSubpages(const ViewContext& subPageCtx)
    : boardConfigSubpage(subPageCtx), diceConfigSubpage(subPageCtx),
      generalConfigSubpage(subPageCtx), puocConfigSubpage(subPageCtx),
      scoreConfigSubpage(subPageCtx), tcConfigSubpage(subPageCtx),
      playerCountRelatedInfoSubpage(subPageCtx)
{
}

std::unique_ptr<AllSubpageInformation> AllSubpages::collect_information()
{
	auto collectedInformation = std::make_unique<AllSubpageInformation>();

	collectedInformation->generalInfo = generalConfigSubpage.collect_information();
	collectedInformation->boardConfig = boardConfigSubpage.collect_information();
	collectedInformation->diceConfig  = diceConfigSubpage.get_entered_dice_config();
	collectedInformation->scoreCtx    = scoreConfigSubpage.get_entered_score_config();
	collectedInformation->puocCtx     = puocConfigSubpage.get_underlying_context();
	collectedInformation->tcCtx       = tcConfigSubpage.get_underlying_context();
	collectedInformation->playerCountRelatedInfo =
	    playerCountRelatedInfoSubpage.get_defined_configs();

	return collectedInformation;
}