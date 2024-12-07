#include "WPCChoosingPage.h"

#include "SagradaWindow.h"

WPCChoosingPage::WPCChoosingPage(const ViewContext& ctx,
                                 std::unique_ptr<Game> _game,
                                 const ai_player_config_c& ai_configs,
                                 size_t localPlayerCount)
    : Page(ctx)
    , label_Title("Choose a Window pattern card!")
    , alignment_Title(0.5, 0.0)
    , button_Quit("Quit")
    , button_Start("Start")
    , game(std::move(_game))
    , wpcSize(330, 300)
    , selectedWPC(nullptr)
    , currLocalPlayerInd(0)
{
    controller = LocalPlayerController::with_players(localPlayerCount,
                                                     ai_configs, *game);
    // commonPageElements = std::make_unique<CommonGameElements>();

    for (auto&& localPlayerID : controller->get_local_player_ids()) {
        add_wpc_for_player(localPlayerID);
    }
    localPlayerWPCChoices.front().show();

    commonPageElements = CommonGameElements::create(
        controller->get_game(), controller->get_local_player_ids(), cardWidth);

    vbox_allCards.pack_start(commonPageElements->tcViews);
    vbox_allCards.pack_start(commonPageElements->puocViews,
                             Gtk::PACK_EXPAND_PADDING, 10);
    vbox_allCards.pack_start(commonPageElements->procViews, Gtk::PACK_SHRINK);

    alignment_Title.add(label_Title);
    alignment_Title.set_size_request(wpcPerRow * wpcSize.width, 70);

    Pango::FontDescription font_desc;
    font_desc.set_style(Pango::STYLE_OBLIQUE);
    font_desc.set_size(Pango::SCALE * 18); // Adjust the size as needed

    // Set the font description on the Gtk::Label
    label_Title.override_font(font_desc);

    button_Start.set_size_request(100, 50);
    button_Start.signal_clicked().connect(
        sigc::mem_fun(*this, &WPCChoosingPage::start_button_clicked));

    button_Quit.signal_clicked().connect(
        sigc::mem_fun(*this, &WPCChoosingPage::quit_button_clicked));
    button_Quit.set_size_request(100, 50);

    hbox_Buttons.pack_start(button_Quit);
    hbox_Buttons.pack_start(button_Start);

    fixed.put(hbox_Buttons, (ctx.size.width - 200) / 2,
              ctx.size.height - 50 - 10);
    fixed.put(alignment_Title, leftMargin, 0);
    fixed.put(vbox_allCards, ctx.size.width / 2 + 60, 20);

    add(fixed);
    show_all_children();
}

void WPCChoosingPage::receive_wpc_click_signal(RectangularBoardView* signaller)
{
    if (selectedWPC) {
        selectedWPC->unselect();
    }
    selectedWPC = signaller;
    selectedWPC->select();
}

void WPCChoosingPage::quit_button_clicked() { ctx.mainWindow.show_home_page(); }

void WPCChoosingPage::start_button_clicked()
{
    if (!selectedWPC) {
        Gtk::MessageDialog dialog {ctx.mainWindow, "No WPC selected!"};
        dialog.set_secondary_text(
            "You have to select a Window pattern card by clicking on it!");
        dialog.run();
        return;
    }

    controller->handle_local_player_WPC_choice(
        controller->get_local_player_ids()[currLocalPlayerInd],
        selectedWPC->get_ctx());

    vbox_allCards.remove(commonPageElements->puocViews);
    vbox_allCards.remove(commonPageElements->tcViews);
    vbox_allCards.remove(commonPageElements->procViews);

    gamePlayingPage = std::make_unique<LocalGamePlayingPage>(
        ctx, std::move(controller), std::move(commonPageElements),
        std::move(game));

    ctx.mainWindow.add_page(*gamePlayingPage);
    ctx.mainWindow.show_page(*gamePlayingPage);

    gamePlayingPage->hide_non_load_page_widgets();
}

void WPCChoosingPage::add_proc_for_player(ID_t localPlayerID)
{
    auto& playerInfo =
        GameInfo::get_player_with_id(controller->get_game(), localPlayerID);
    commonPageElements->localPlayerProcCards.push_back(std::make_unique<Image>(
        procImagePaths.at(playerInfo->PrOC->get_color()),
        SizeInPixels {cardWidth, size_t(cardWidth / procWidthToHeightRatio)}));
    commonPageElements->procViews.pack_start(
        *commonPageElements->localPlayerProcCards.back());
}

void WPCChoosingPage::add_wpc_for_player(ID_t localPlayerID)
{
    auto&& [wpcBegin, wpcEnd] =
        controller->get_wpc_choices_for_local_player_with_id(localPlayerID);
    size_t wpcCount = std::distance(wpcBegin, wpcEnd);
    localPlayerWPCChoices.push_back(
        Gtk::Table {wpcPerRow, guint(wpcCount / wpcPerRow)});
    Gtk::Table& lastTable = localPlayerWPCChoices.back();

    size_t ind = 0;
    for (auto it = wpcBegin; it != wpcEnd; ++it) {
        wpcChoices.push_back(
            std::make_unique<RectangularBoardView>(**it, wpcSize));
        auto& wpc = *wpcChoices.back();
        wpc.subscribe(this);
        wpc.set_size_request(wpcSize.width, wpcSize.height);
        int row = ind / wpcPerRow;
        int col = ind % wpcPerRow;
        lastTable.attach(wpc, col, col + 1, row, row + 1);
        ++ind;
    }

    lastTable.set_size_request(2 * wpcSize.width, 2 * wpcSize.height);
    lastTable.hide();
    fixed.put(lastTable, leftMargin,
              (ctx.size.height - 2 * wpcSize.height) / 2);
}