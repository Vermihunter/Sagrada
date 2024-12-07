#ifndef COMMON_GAME_CONFIG_ELEMENTS_h
#define COMMON_GAME_CONFIG_ELEMENTS_h

#include <gtkmm.h>

#include "AI_TypeSpecifierView.h"
#include "DefinedAIPlayers.h"
#include "GameCache.h"
#include "GameConfig.h"
#include "GameConfigurationWidgets.h"
#include "LabeledWidget.h"
#include "NumberEntry.h"
#include "ViewContext.h"

struct CommonGameConfigElements
{
  private:
    ViewContext ctx;

  public:
    CommonGameConfigElements(const ViewContext& ctx,
                             const std::string& pageName,
                             size_t _localPlayerCount);

    void add_all_to_vbox();

    Gtk::Fixed fixed;
    Gtk::VBox vbox_AllWidgets;

    LabeledWidget<NumberEntry> lne_Seed;
    Gtk::Button button_Start;
    Gtk::Button button_Back;

    Gtk::Label label_SelectAIPlayers;
    Gtk::HBox hbox_AiPlayers;
    std::vector<Gtk::VBox> AI_PlayerTypeConfigs;
    std::vector<Gtk::ComboBoxText> AI_playerConfigBoxes;
    std::vector<std::unique_ptr<AI_TypeSpecifierView>> AI_TypesSpecifiers;
    Gtk::CheckButton chb_GameDeterministic;

    GameConfigurationWidgets gameCtxWidgets;

    game_config_t get_selected_information(bool ignoreNotEnteredSeed = false);

  private:
    void player_count_changed();

    void add_new_ai_config_box();
    void remove_last_config_box();

    void ai_type_changed(size_t ind);
    std::unique_ptr<AI_TypeSpecifierView> get_selected_type_specifier(
        size_t ind);
};

#endif // COMMON_GAME_CONFIG_ELEMENTS_h