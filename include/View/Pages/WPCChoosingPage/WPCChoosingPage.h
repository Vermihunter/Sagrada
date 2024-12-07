#ifndef WPC_CHOOSING_PAGE_H
#define WPC_CHOOSING_PAGE_H

#include <gtkmm.h>

#include "CommonGameViewElements.h"
#include "Game.h"
#include "IBoardViewSignallable.h"
#include "Image.h"
#include "LocalGamePlayingPage.h"
#include "LocalPlayerController.h"
#include "Page.h"
#include "RectangularBoardView.h"
#include "ToolCardView.h"
#include "Typedefs.h"

class WPCChoosingPage : public Page, public IBoardViewSignallable
{
    static constexpr size_t cardWidth = 155;

  public:
    WPCChoosingPage(const ViewContext& ctx, std::unique_ptr<Game> game,
                    const ai_player_config_c& ai_configs,
                    size_t localPlayerCount);

    virtual void receive_wpc_click_signal(
        RectangularBoardView* signaller) override;

  private:
    ai_player_config_c ai_configs;

    local_player_controller_t controller;

    Gtk::Fixed fixed;
    Gtk::Label label_Title;
    Gtk::Alignment alignment_Title;

    Gtk::HBox hbox_Buttons;
    Gtk::Button button_Quit;
    Gtk::Button button_Start;

    // Contains a Table for all the local players
    std::vector<Gtk::Table> localPlayerWPCChoices;
    // All the WPC stored here
    std::vector<std::unique_ptr<RectangularBoardView>> wpcChoices;

    std::unique_ptr<CommonGameElements> commonPageElements;
    std::unique_ptr<Game> game;

    Gtk::VBox vbox_allCards;
    SizeInPixels wpcSize;

    // Currently selected WPC
    RectangularBoardView* selectedWPC;

    size_t currLocalPlayerInd;

    std::unique_ptr<GamePlayingPage> gamePlayingPage;

    constexpr static size_t leftMargin = 10;
    constexpr static size_t wpcPerRow = 2;

    void quit_button_clicked();
    void start_button_clicked();
    void add_proc_for_player(ID_t localPlayerID);
    void add_wpc_for_player(ID_t localPlayerID);
};

#endif // WPC_CHOOSING_PAGE_H
