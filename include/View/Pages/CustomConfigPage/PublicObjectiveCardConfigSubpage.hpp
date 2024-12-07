#ifndef PUBLICOBJECTIVECARDCONFIGSUBPAGE_HPP
#define PUBLICOBJECTIVECARDCONFIGSUBPAGE_HPP

#include <memory>
#include <ranges>
#include <vector>

#include "CustomConfigPageSubpage.h"
#include "PublicObjectiveCardConfig.h"
#include "PublicObjectiveCardGameContext.h"
#include "PublicObjectiveCardImages.h"

class PublicObjectiveCardConfigSubpage : public CustomConfigPageSubpage
{
    static constexpr size_t configPerRow = 6;

  public:
    PublicObjectiveCardConfigSubpage(const ViewContext& ctx)
        : CustomConfigPageSubpage(ctx)
        , lne_CardsPerGame("Cards per game")
        , alignment_CardPerGameMiddle(0.5, 0, 0)
        , alignment_CardPacksMiddle(0.5, 0, 0)
    {
        auto defaultPuocCtx = DefaultGameCreator::get_default_puoc_context();

        size_t ind = 0;
        for (auto&& puocImage : PublicObjectiveCardImages::get_all()) {
            puoc_etype type = puoc_etype(ind);
            puocConfigs.push_back(std::make_unique<PublicObjectiveCardConfig>(
                *puocImage, type,
                defaultPuocCtx->get_context_for(type)
                    ->get_satisfaction_value()));

            if (ind < configPerRow) {
                hbox_CardsPack1.pack_start(*puocConfigs.back(),
                                           Gtk::PACK_SHRINK, 5);
            }
            else {
                hbox_CardsPack2.pack_start(*puocConfigs.back(),
                                           Gtk::PACK_SHRINK, 5);
            }
            ++ind;
        }

        vbox_CardPacks.pack_start(hbox_CardsPack1, Gtk::PACK_SHRINK, 5);
        vbox_CardPacks.pack_start(hbox_CardsPack2, Gtk::PACK_SHRINK, 5);

        alignment_CardPacksMiddle.add(vbox_CardPacks);
        alignment_CardPacksMiddle.set_size_request(ctx.size.width, -1);

        lne_CardsPerGame.widget.set_text(std::to_string(defaultPuocPerGame));

        put(alignment_CardPacksMiddle, 0, 0);
        put(lne_CardsPerGame, 900, ctx.size.height - 220);
    }

    puoc_context_t get_underlying_context()
    {
        return std::make_unique<PublicObjectiveCardGameContext>(
            lne_CardsPerGame.widget.get_underlying_number("PuOC per game"),
            get_selected_card_configs());
    }

    puoc_config_c get_selected_card_configs()
    {
        auto selectedFilter = std::views::filter(
            [](const std::unique_ptr<PublicObjectiveCardConfig>& obj) {
                return obj->is_selected();
            });

        auto puocConfigGetter = std::views::transform(
            [](const std::unique_ptr<PublicObjectiveCardConfig>& obj) {
                return std::make_pair(
                    obj->type,
                    obj->lne_SatisfactionValue.widget.get_underlying_number(
                        "Satisfaction value"));
            });

        puoc_config_c selectedConfigs;

        std::ranges::copy(puocConfigs | selectedFilter | puocConfigGetter,
                          std::back_inserter(selectedConfigs));

        return selectedConfigs;
    }

  private:
    std::vector<std::unique_ptr<PublicObjectiveCardConfig>> puocConfigs;

    Gtk::HBox hbox_CardsPack1;
    Gtk::HBox hbox_CardsPack2;

    LabeledWidget<NumberEntry> lne_CardsPerGame;
    Gtk::Alignment alignment_CardPerGameMiddle;

    Gtk::Alignment alignment_CardPacksMiddle;
    Gtk::VBox vbox_CardPacks;
};

#endif // PUBLICOBJECTIVECARDCONFIGSUBPAGE_HPP