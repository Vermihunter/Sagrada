#ifndef DICE_CONFIG_SUBPAGE_HPP
#define DICE_CONFIG_SUBPAGE_HPP

#include <vector>

#include "CustomConfigPageSubpage.h"
#include "DefaultGameConstants.h"
#include "DiceConfig.h"
#include "OneColoredDieConfig.h"
#include "StringAttributes.h"

class DiceConfigSubpage : public CustomConfigPageSubpage
{
  public:
    DiceConfigSubpage(const ViewContext& ctx) : CustomConfigPageSubpage(ctx)
    {
        for (size_t i = 0; i < size_t(color_t::COUNT); ++i) {
            diceByColors.push_back(
                OneColoredDieConfig {color_t(i), defaultDicePerColor});
            vbox_dice.pack_start(diceByColors.back(), Gtk::PACK_SHRINK, 10);
        }

        vbox_dice.set_size_request(400, 100);
        put(vbox_dice, (ctx.size.width - 400) / 2, 80);
    }

    dice_config_c get_entered_dice_config()
    {
        dice_config_c config = std::make_unique<DiceConfig>();
        for (auto&& dieConfig : diceByColors) {
            config->diceByColor[dieConfig.dieColor].diceCount =
                dieConfig.widget.get_underlying_number(
                    color_to_string.at(dieConfig.dieColor) + " die count");
        }

        return config;
    }

  private:
    std::vector<OneColoredDieConfig> diceByColors;
    Gtk::VBox vbox_dice;
};

#endif // DICE_CONFIG_SUBPAGE_HPP