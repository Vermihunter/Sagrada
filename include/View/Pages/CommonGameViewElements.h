#ifndef COMMON_GAME_ELEMENTS_H
#define COMMON_GAME_ELEMENTS_H

#include <gtkmm.h>

#include <memory>

#include "GameInfo.h"
#include "Image.h"
#include "PrivateObjectiveCardPathsByType.h"
#include "PrivateObjectiveCardViewConstants.h"
#include "PublicObjectiveCardPathsByType.h"
#include "PublicObjectiveCardViewConstants.h"
#include "ToolCardView.h"
#include "ToolCardViewConstants.h"
#include "ToolCardViewContainer.h"
#include "Typedefs.h"

struct CommonGameElements
{
    CommonGameElements(Game& g, size_t cardWidth) : tcViews(g, cardWidth) {}

    static auto create(Game& g, const std::vector<ID_t> localPlayerIds,
                       size_t cardWidth)
    {
        auto instance = std::make_unique<CommonGameElements>(g, cardWidth);

        for (auto&& gamePuoc : g.get_ctx()->publicObjectiveCards) {
            const std::string& path = puocImagePaths.at(gamePuoc->get_type());
            instance->puocCards.push_back(std::make_unique<Image>(
                path,
                SizeInPixels {cardWidth,
                              size_t(cardWidth / puocWidthToHeightRatio)}));
            instance->puocViews.pack_start(*instance->puocCards.back(),
                                           Gtk::PACK_EXPAND_PADDING, 10);
        }

        for (auto&& localPlayerID : localPlayerIds) {
            auto& playerInfo = GameInfo::get_player_with_id(g, localPlayerID);
            instance->localPlayerProcCards.push_back(std::make_unique<Image>(
                procImagePaths.at(playerInfo->PrOC->get_color()),
                SizeInPixels {cardWidth,
                              size_t(cardWidth / procWidthToHeightRatio)}));
            instance->procViews.pack_start(
                *instance->localPlayerProcCards.back());
        }

        return instance;
    }

    void hide()
    {
        puocViews.hide();
        procViews.hide();
        tcViews.hide();
    }

    void show()
    {
        puocViews.show();
        procViews.show();
        tcViews.show();
    }

    // Public objective cards
    std::vector<std::unique_ptr<Image>> puocCards;
    Gtk::HBox puocViews;

    // Private objective cards
    std::vector<std::unique_ptr<Image>> localPlayerProcCards;
    Gtk::HBox procViews;

    // Tool cards
    ToolCardViewContainer tcViews;
};

#endif // COMMON_GAME_ELEMENTS_H