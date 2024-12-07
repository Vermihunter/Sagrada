#ifndef PLAYER_COUNT_RELATED_INFORMATION_SUBPAGE_H
#define PLAYER_COUNT_RELATED_INFORMATION_SUBPAGE_H

#include "CustomConfigPageSubpage.h"
#include "LabeledWidget.h"
#include "NumberEntry.h"
#include "PlayerCountRelatedConfig.hpp"
#include "PlayerCountRelatedGameInformation.h"

class PlayerCountRelatedInformationSubpage : public CustomConfigPageSubpage
{
  public:
    PlayerCountRelatedInformationSubpage(const ViewContext& ctx)
        : CustomConfigPageSubpage(ctx)
        , lne_PlayerCount("Enter player count")
        , lne_DiceCount("Enter dice count per round")
        , hbox_TitleLabels(true)
        , label_PlayerCount("Player count")
        , label_DicePerRound("Dice per round")
        , label_Settings("Settings")
        , label_Delete("Delete")
        , button_AddNew("+ Add new configuration")

    {
        button_AddNew.signal_clicked().connect(sigc::mem_fun(
            *this,
            &PlayerCountRelatedInformationSubpage::add_new_button_clicked));

        label_PlayerCount.set_sensitive(false);
        label_DicePerRound.set_sensitive(false);
        label_Settings.set_sensitive(false);
        label_Delete.set_sensitive(false);

        hbox_TitleLabels.pack_start(label_PlayerCount);
        hbox_TitleLabels.pack_start(label_DicePerRound);
        hbox_TitleLabels.pack_start(label_Settings);
        hbox_TitleLabels.pack_start(label_Delete);

        hbox_NewConfig.pack_start(lne_PlayerCount);
        hbox_NewConfig.pack_start(lne_DiceCount);

        vbox_AllConfigs.pack_start(hbox_TitleLabels);
        vbox_AllConfigs.pack_start(hbox_NewConfig);
        vbox_AllConfigs.pack_start(button_AddNew);

        auto defaultPlayerCountRelatedConfig =
            DefaultGameCreator::get_default_player_count_related_config();
        for (auto&& config : defaultPlayerCountRelatedConfig) {
            add_config(config.playerCount, config.diceCount);
        }

        vbox_AllConfigs.set_size_request(600, -1);
        put(vbox_AllConfigs, (ctx.size.width - 600) / 2, 50);
    }

    std::vector<PlayerCountRelatedGameInformation> get_defined_configs()
    {
        auto configTransformer = std::views::transform(
            [](const std::unique_ptr<PlayerCountRelatedConfig>& obj) {
                return PlayerCountRelatedGameInformation {
                    .playerCount = obj->get_player_count(),
                    .diceCount = obj->get_dice_per_round_count()};
            });

        std::vector<PlayerCountRelatedGameInformation> enteredConfigs;

        std::ranges::copy(configs | configTransformer,
                          std::back_inserter(enteredConfigs));

        return enteredConfigs;
    }

  protected:
    virtual void on_show() override
    {
        CustomConfigPageSubpage::on_show();
        hbox_TitleLabels.hide();
    }

  private:
    LabeledWidget<NumberEntry> lne_PlayerCount;
    LabeledWidget<NumberEntry> lne_DiceCount;
    Gtk::HBox hbox_NewConfig;

    std::vector<std::unique_ptr<PlayerCountRelatedConfig>> configs;

    Gtk::HBox hbox_TitleLabels;
    Gtk::Button label_PlayerCount;
    Gtk::Button label_DicePerRound;
    Gtk::Button label_Settings;
    Gtk::Button label_Delete;

    Gtk::Button button_AddNew;
    Gtk::VBox vbox_AllConfigs;

    void reset_attributes()
    {
        lne_PlayerCount.widget.set_text("");
        lne_DiceCount.widget.set_text("");
    }

    void add_new_button_clicked()
    {
        std::string playerCountStr = lne_PlayerCount.widget.get_text();
        std::string diceCountStr = lne_DiceCount.widget.get_text();

        if (playerCountStr.empty() || diceCountStr.empty()) {
            // display error message
            return;
        }

        size_t playerCount = std::stoull(playerCountStr);
        size_t diceCount = std::stoull(diceCountStr);
        add_config(playerCount, diceCount);

        reset_attributes();
    }

    void add_config(size_t playerCount, size_t diceCount)
    {
        const auto configByPlayerCount =
            [=](const std::unique_ptr<PlayerCountRelatedConfig>& config) {
                return config->get_player_count() == playerCount;
            };

        if (std::find_if(configs.begin(), configs.end(), configByPlayerCount) !=
            configs.end()) {
            // display error message
            reset_attributes();
            return;
        }

        if (configs.empty()) {
            hbox_TitleLabels.show();
        }

        auto newConfig =
            std::make_unique<PlayerCountRelatedConfig>(playerCount, diceCount);
        newConfig->delete_button().signal_clicked().connect(sigc::bind(
            sigc::mem_fun(*this, &PlayerCountRelatedInformationSubpage::
                                     config_delete_button_clicked),
            newConfig.get()));

        newConfig->settings_button().signal_clicked().connect(sigc::bind(
            sigc::mem_fun(*this, &PlayerCountRelatedInformationSubpage::
                                     config_settings_button_clicked),
            newConfig.get()));

        vbox_AllConfigs.pack_start(*newConfig);
        vbox_AllConfigs.reorder_child(*newConfig, configs.size() + 1);
        vbox_AllConfigs.show_all_children();
        configs.push_back(std::move(newConfig));
    }

    void config_delete_button_clicked(PlayerCountRelatedConfig* sender)
    {
        vbox_AllConfigs.remove(*sender);

        const auto findByRaw =
            [&](const std::unique_ptr<PlayerCountRelatedConfig>& config) {
                return config.get() == sender;
            };

        configs.erase(
            std::find_if(configs.cbegin(), configs.cend(), findByRaw));
        if (configs.empty()) {
            hbox_TitleLabels.hide();
        }
    }

    void config_settings_button_clicked(PlayerCountRelatedConfig* sender)
    {
        lne_PlayerCount.widget.set_text(
            std::to_string(sender->get_player_count()));
        lne_DiceCount.widget.set_text(
            std::to_string(sender->get_dice_per_round_count()));
        config_delete_button_clicked(sender);
    }
};

#endif // PLAYER_COUNT_RELATED_INFORMATION_SUBPAGE_H