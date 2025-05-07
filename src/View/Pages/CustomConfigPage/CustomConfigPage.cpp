#include "CustomConfigPage.h"

#include "AllSubpages.h"
#include "Constants.h"
#include "GameCache.h"
#include "GameContextJson.h"
#include "SagradaWindow.h"

#include <fstream>

CustomConfigPage::CustomConfigPage(const ViewContext& ctx)
    : Page(ctx), tryLabel("first page"), hbox_SaveAndQuitButtons(true), button_Save("Save"),
      button_Quit("Quit")
{
	ViewContext subpageCtx{
	    SizeInPixels{ctx.size.width, SizeInPixels::pixel_size_t(0.9 * ctx.size.height)},
	    ctx.upperLeft, ctx.mainWindow}; // * 0.9
	allSubpages = std::make_unique<AllSubpages>(subpageCtx);

	notebook_SettingsByParts.insert_page(allSubpages->generalConfigSubpage, "General config", 0);
	notebook_SettingsByParts.insert_page(allSubpages->boardConfigSubpage, "Board config", 1);
	notebook_SettingsByParts.insert_page(allSubpages->diceConfigSubpage, "Dice config", 3);
	notebook_SettingsByParts.insert_page(allSubpages->scoreConfigSubpage, "Score config", 4);
	notebook_SettingsByParts.insert_page(allSubpages->playerCountRelatedInfoSubpage,
	                                     "Player count related information", 5);
	notebook_SettingsByParts.insert_page(allSubpages->puocConfigSubpage,
	                                     "Public objective card config", 6);
	notebook_SettingsByParts.insert_page(allSubpages->tcConfigSubpage, "Tool card config", 7);

	button_Quit.signal_clicked().connect(
	    sigc::mem_fun(*this, &CustomConfigPage::quit_button_clicked));

	button_Save.signal_clicked().connect(
	    sigc::mem_fun(*this, &CustomConfigPage::save_button_clicked));

	set_size_request(ctx.size.width, ctx.size.height);
	fixed.put(notebook_SettingsByParts, 0, 0);
	notebook_SettingsByParts.set_size_request(ctx.size.width,
	                                          ctx.size.height); //

	hbox_SaveAndQuitButtons.pack_start(button_Save);
	hbox_SaveAndQuitButtons.pack_start(button_Quit);

	hbox_SaveAndQuitButtons.set_size_request(150, 40);
	fixed.put(hbox_SaveAndQuitButtons, (ctx.size.width - 150) / 2, ctx.size.height - 40 - 5);

	add(fixed);
	show_all_children();
}

void CustomConfigPage::save_button_clicked()
{
	std::unique_ptr<AllSubpageInformation> collectedInformation;
	try {
		collectedInformation = allSubpages->collect_information();
	}
	catch (const InformationNotProvidedException& e) {
		return ctx.mainWindow.display_error_message("Missing information!", e.what());
	}

	if (collectedInformation->playerCountRelatedInfo.empty()) {
		return ctx.mainWindow.display_error_message("Missing information!",
		                                            "No player configuration provided!");
	}

	if (collectedInformation->generalInfo->contextName.empty()) {
		return ctx.mainWindow.display_error_message("Missing information!",
		                                            "Enter a name for the game context!");
	}

	cached_game_ctx_builder_t gameCtxBuilder;
	try {
		gameCtxBuilder = GameContextBuilder::create(
		    collectedInformation->playerCountRelatedInfo, std::move(collectedInformation->puocCtx),
		    std::move(collectedInformation->tcCtx), collectedInformation->boardConfig->boardConfig);

		gameCtxBuilder->add_dice_config(std::move(collectedInformation->diceConfig))
		    .add_wpc_choice_per_player(collectedInformation->generalInfo->wpcPerPlayer)
		    .add_number_of_rounds(collectedInformation->generalInfo->numberOfRounds)
		    .set_name(collectedInformation->generalInfo->contextName)
		    .add_score_ctx(std::move(collectedInformation->scoreCtx))
		    .add_selectable_wpc(collectedInformation->boardConfig->selectableWpc);
	}
	catch (GameContextBuildingException& buildingException) {
		return ctx.mainWindow.display_error_message("Invalid configuration!",
		                                            buildingException.what());
	}

	auto existsCtxBuilderWithName = [&](auto& buildersByPlayerCount) {
		return std::find_if(buildersByPlayerCount.second.begin(),
		                    buildersByPlayerCount.second.end(),
		                    [&](cached_game_ctx_builder_t& builder) {
			                    return builder->get_name() == gameCtxBuilder->get_name();
		                    }) != buildersByPlayerCount.second.end();
	};

	auto& definedCtxBuilders = GameCache::get().get_defined_game_contexts();
	if (std::find_if(definedCtxBuilders.begin(), definedCtxBuilders.end(),
	                 existsCtxBuilderWithName) != definedCtxBuilders.end()) {
		return ctx.mainWindow.display_error_message("Invalid configuration!",
		                                            "A context with the same name already exists!");
	}

	for (auto&& playerRelatedInfo : collectedInformation->playerCountRelatedInfo) {
		auto playerCountCtxIt = definedCtxBuilders.find(playerRelatedInfo.playerCount);
		if (playerCountCtxIt == definedCtxBuilders.end()) {
			definedCtxBuilders.insert(
			    {playerRelatedInfo.playerCount, cached_game_ctx_builder_c{gameCtxBuilder}});
		}
		else {
			playerCountCtxIt->second.push_back(gameCtxBuilder);
		}
	}

	json          builderJson = *gameCtxBuilder;
	std::ofstream file(gameConfigDir + "/" + collectedInformation->generalInfo->contextName +
	                   ".json");
	file << std::setw(4) << builderJson;

	ctx.mainWindow.display_error_message("Context successfully saved!");
	ctx.mainWindow.show_home_page();
}

void CustomConfigPage::quit_button_clicked()
{
	ctx.mainWindow.show_home_page();
}

CustomConfigPage::~CustomConfigPage() {}