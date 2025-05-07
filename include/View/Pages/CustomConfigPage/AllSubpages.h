#ifndef ALL_SUBPAGES_H
#define ALL_SUBPAGES_H

#include "BoardConfigSubpage.hpp"
#include "DiceConfigSubpage.hpp"
#include "GeneralConfigSubpage.hpp"
#include "PlayerCountRelatedInformationSubpage.hpp"
#include "PublicObjectiveCardConfigSubpage.hpp"
#include "ScoreConfigSubpage.hpp"
#include "ToolCardConfigSubpage.hpp"
#include "ViewContext.h"

#include <memory>

class BoardConfigSubpage;
class DiceConfigSubpage;
class GeneralConfigSubpage;
class PublicObjectiveCardConfigSubpage;
class ScoreConfigSubpage;
class ToolCardConfigSubpage;
class WPCConfigSubpage;
class PlayerCountRelatedInformationSubpage;

struct AllSubpageInformation {
	general_config_subpage_info_t                  generalInfo;
	board_config_subpage_info_t                    boardConfig;
	dice_config_c                                  diceConfig;
	score_ctx_t                                    scoreCtx;
	puoc_context_t                                 puocCtx;
	tc_context_t                                   tcCtx;
	std::vector<PlayerCountRelatedGameInformation> playerCountRelatedInfo;
};

struct AllSubpages {
	AllSubpages(const ViewContext& subPageCtx);

	std::unique_ptr<AllSubpageInformation> collect_information();

	BoardConfigSubpage                   boardConfigSubpage;
	DiceConfigSubpage                    diceConfigSubpage;
	GeneralConfigSubpage                 generalConfigSubpage;
	PublicObjectiveCardConfigSubpage     puocConfigSubpage;
	ScoreConfigSubpage                   scoreConfigSubpage;
	ToolCardConfigSubpage                tcConfigSubpage;
	PlayerCountRelatedInformationSubpage playerCountRelatedInfoSubpage;
};

#endif // ALL_SUBPAGES_H