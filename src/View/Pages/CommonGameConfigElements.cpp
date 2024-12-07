#include "CommonGameConfigElements.h"

#include "FirstPlayerConfig.h"
#include "MCTSAI_TypeSpecifierView.h"
#include "MinimaxAI_TypeSpecifierView.h"
#include "RandomPlayerConfig.h"
#include "RulesBasedAI_TypeSpecifierView.h"
#include "SagradaWindow.h"
#include "SimpleAI_TypeSpecifierView.h"

CommonGameConfigElements::CommonGameConfigElements(const ViewContext& _ctx,
                                                   const std::string& pageName,
                                                   size_t _localPlayerCount)
    : ctx(_ctx)
    , lne_Seed("Enter a random seed for the " + pageName)
    , button_Start("Start")
    , button_Back("Back")
    , label_SelectAIPlayers("Select AI players")
    , chb_GameDeterministic("Deterministic game")
    , gameCtxWidgets(ctx, GameCache::get().get_defined_game_contexts(),
                     _localPlayerCount)

{
    gameCtxWidgets.get_player_count_cb().signal_changed().connect(
        sigc::mem_fun(*this, &CommonGameConfigElements::player_count_changed));

    button_Back.signal_clicked().connect(
        sigc::mem_fun(ctx.mainWindow, &SagradaWindow::show_home_page));

    chb_GameDeterministic.set_margin_top(40);
    label_SelectAIPlayers.set_margin_top(20);

    vbox_AllWidgets.set_size_request(400, 150);

    fixed.put(vbox_AllWidgets, (ctx.size.width - 400) / 2,
              ctx.size.height / 2 - 67);

    size_t selectedPlayerCount =
        std::stoul(gameCtxWidgets.get_player_count_cb().get_active_text());
    for (size_t i = 0; i < selectedPlayerCount; ++i) {
        add_new_ai_config_box();
    }
}

void CommonGameConfigElements::add_all_to_vbox()
{
    vbox_AllWidgets.pack_start(lne_Seed);
    vbox_AllWidgets.pack_start(gameCtxWidgets);
    vbox_AllWidgets.pack_start(label_SelectAIPlayers);
    vbox_AllWidgets.pack_start(hbox_AiPlayers);
    vbox_AllWidgets.pack_start(chb_GameDeterministic);
    vbox_AllWidgets.pack_start(button_Start);
    vbox_AllWidgets.pack_start(button_Back);
}

game_config_t CommonGameConfigElements::get_selected_information(
    bool ignoreNotEnteredSeed)
{
    auto commonGameConfigInfo = std::make_unique<GameConfig>();
    try {
        commonGameConfigInfo->gameCtxBuilder =
            gameCtxWidgets.get_context_builder();
    }
    catch (const std::exception& e) {
        ctx.mainWindow.display_error_message("Invalid configuration!",
                                             e.what());
        return nullptr;
    }

    try {
        commonGameConfigInfo->randomSeed =
            lne_Seed.widget.get_underlying_number("Seed");
    }
    catch (const std::exception& e) {
        if (!ignoreNotEnteredSeed) {
            ctx.mainWindow.display_error_message("Invalid configuration!",
                                                 e.what());
            return nullptr;
        }
    }
    commonGameConfigInfo->gameDeterministic =
        chb_GameDeterministic.get_active();

    for (auto&& AI_Chooser : AI_TypesSpecifiers) {
        commonGameConfigInfo->players.push_back(AI_Chooser->create_player());
    }

    return commonGameConfigInfo;
}

void CommonGameConfigElements::player_count_changed()
{
    int selectedPlayerCount =
        std::stoi(gameCtxWidgets.get_player_count_cb().get_active_text());
    int extraPlayerCount =
        std::abs(int(AI_playerConfigBoxes.size()) - selectedPlayerCount);
    bool addExtra = (size_t(selectedPlayerCount) > AI_playerConfigBoxes.size());

    for (int i = 0; i < extraPlayerCount; ++i) {
        if (addExtra)
            add_new_ai_config_box();
        else
            remove_last_config_box();
    }
}

std::unique_ptr<AI_TypeSpecifierView>
CommonGameConfigElements::get_selected_type_specifier(size_t ind)
{
    std::unique_ptr<AI_TypeSpecifierView> result;
    int selectedAITypeIndex = AI_playerConfigBoxes[ind].get_active_row_number();

    switch (selectedAITypeIndex) {
        case 0:
            result = std::make_unique<
                SimpleAI_TypeSpecifierView<RandomPlayerConfig>>();
            break;
        case 1:
            result = std::make_unique<
                SimpleAI_TypeSpecifierView<FirstPlayerConfig>>();
            break;
        case 2:
            /* code */
            result = std::make_unique<MinimaxAI_TypeSpecifierView>();
            break;
        case 3:
            result = std::make_unique<RulesBasedAI_TypeSpecifierView>();
            break;
        case 4:
            /* code */
            result = std::make_unique<MCTSAI_TypeSpecifierView>();
            break;
        // nothing is selected
        case -1:

            break;
        default:
            break;
    }

    return result;
}

void CommonGameConfigElements::ai_type_changed(size_t ind)
{
    AI_PlayerTypeConfigs[ind].remove(*AI_TypesSpecifiers[ind]);
    AI_TypesSpecifiers[ind] = get_selected_type_specifier(ind);
    AI_PlayerTypeConfigs[ind].pack_start(*AI_TypesSpecifiers[ind]);
    AI_PlayerTypeConfigs[ind].show_all_children();
}

void CommonGameConfigElements::add_new_ai_config_box()
{
    AI_playerConfigBoxes.push_back(Gtk::ComboBoxText {});
    AI_PlayerTypeConfigs.push_back(Gtk::VBox {});

    Gtk::ComboBoxText& lastAdded = AI_playerConfigBoxes.back();
    for (auto&& ai_playerConfig : DefinedAIPlayers::get()) {
        lastAdded.append(ai_playerConfig->get_base_name());
    }

    AI_TypesSpecifiers.push_back(
        std::make_unique<SimpleAI_TypeSpecifierView<RandomPlayerConfig>>());

    AI_PlayerTypeConfigs.back().pack_start(lastAdded);
    AI_PlayerTypeConfigs.back().pack_start(*AI_TypesSpecifiers.back());
    lastAdded.set_active(0);

    lastAdded.signal_changed().connect(sigc::bind(
        sigc::mem_fun(*this, &CommonGameConfigElements::ai_type_changed),
        AI_playerConfigBoxes.size() - 1));

    hbox_AiPlayers.pack_start(AI_PlayerTypeConfigs.back());
    hbox_AiPlayers.show_all_children();
}

void CommonGameConfigElements::remove_last_config_box()
{
    auto& lastAIConfigButton =
        AI_playerConfigBoxes[AI_playerConfigBoxes.size() - 1];
    hbox_AiPlayers.remove(lastAIConfigButton);
    AI_TypesSpecifiers.pop_back();

    AI_playerConfigBoxes.pop_back();
    hbox_AiPlayers.show_all_children();
}
