#ifndef PLAYER_COUNT_RELATED_CONFIG_HPP
#define PLAYER_COUNT_RELATED_CONFIG_HPP

#include <gtkmm.h>

class PlayerCountRelatedConfig : public Gtk::HBox
{
  public:
    PlayerCountRelatedConfig(size_t _playerCount, size_t _diceCount)
        : Gtk::HBox(/* allColumnsSameWidth = */ true)
        , playerCount(_playerCount)
        , diceCount(_diceCount)
        , label_PlayerCount(std::to_string(playerCount))
        , label_DiceCount(std::to_string(diceCount))
    {
        button_Settings.set_image_from_icon_name("applications-system");
        button_Delete.set_image_from_icon_name("user-trash");

        pack_start(label_PlayerCount);
        pack_start(label_DiceCount);
        pack_start(button_Settings);
        pack_start(button_Delete);
    }

    auto& delete_button() { return button_Delete; }
    auto& settings_button() { return button_Settings; }
    size_t get_player_count() const { return playerCount; }
    size_t get_dice_per_round_count() const { return diceCount; }

  private:
    size_t playerCount;
    size_t diceCount;

    Gtk::Label label_PlayerCount;
    Gtk::Label label_DiceCount;
    Gtk::Button button_Settings;
    Gtk::Button button_Delete;
};

#endif // PLAYER_COUNT_RELATED_CONFIG_HPP