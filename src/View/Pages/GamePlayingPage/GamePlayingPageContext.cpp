#include "GamePlayingPageContext.h"

#include "GamePlayingPage.h"

GamePlayingPageContext::GamePlayingPageContext(
    const ViewContext& _ctx, ControllerWithAIPlayer& controller,
    std::unique_ptr<CommonGameElements> _commonGameElements)
    : roundTrackView(SizeInPixels {50, 50},
                     controller.get_game().get_ctx()->roundTrack.get_dice(), 8)
    , currRoundsDice(
          SizeInPixels {50, 50},
          controller.get_game().get_curr_state()->currentRoundRemainingDice)
    , playerOrderDisplayer(*controller.get_game().get_curr_state(),
                           controller.get_game().get_ctx()->players,
                           SizeInPixels {_ctx.size.width, 70})
    , commonGameElements(std::move(_commonGameElements))
    , button_Quit("Quit")
    , currentlyShownCards(nullptr)
    , roundNumberView(controller.get_game().get_curr_state()->roundNumber,
                      v_constants::roundNumberSize)
    , ctx(_ctx)
{
    SizeInPixels wpcSize {330, 300};
    size_t ind = 0;

    for (auto&& player : controller.get_game().get_ctx()->players) {
        auto playerWpc = std::make_unique<RectangularBoardView>(
            *player->board->get_ctx(), wpcSize,
            player.get()); //
        size_t extraTopSpace =
            (ctx.size.height - (2 * playerWpc->get_size().height)) / 2;
        fixed.put(*playerWpc, (ind % 2) * (ctx.size.width - wpcSize.width),
                  extraTopSpace + (ind / 2) * (playerWpc->get_size().height));
        playerBoards.push_back(std::move(playerWpc));
        ++ind;
    }

    button_Quit.set_size_request(100, 40);
    commonGameElements->tcViews.construct_common_drawing_area();

    SizeInPixels iconSizes {50, 50};
    puocIcon = CardIcon::create_puoc_icon(
        iconSizes, commonGameElements->puocViews, currentlyShownCards);
    tcIcon = CardIcon::create_tc_icon(iconSizes, commonGameElements->tcViews,
                                      currentlyShownCards);
    procIcon = CardIcon::create_proc_icon(
        iconSizes, commonGameElements->procViews, currentlyShownCards);
    hbox_icons.pack_start(*puocIcon);
    hbox_icons.pack_start(*tcIcon, Gtk::PACK_SHRINK, 10);
    hbox_icons.pack_start(*procIcon);

    const auto& roundTrackSize = roundTrackView.get_size();
    const auto& diceSize = currRoundsDice.get_size();

    if (roundTrackSize.width > diceSize.width) {
        currRoundsDice.set_margin_left((roundTrackSize.width - diceSize.width) /
                                       2);
    }
    else {
        roundTrackView.set_margin_left((diceSize.width - roundTrackSize.width) /
                                       2);
    }

    size_t cardIconY = 105 + roundTrackSize.height + diceSize.height + 40 + 10;

    size_t longestWidth = std::max(roundTrackView.get_size().width,
                                   currRoundsDice.get_size().width);

    vbox_Moves.pack_start(roundTrackView);
    vbox_Moves.pack_start(currRoundsDice);

    fixed.put(playerOrderDisplayer, 0, 0);
    fixed.put(vbox_Moves, (ctx.size.width - longestWidth) / 2, 105);
    fixed.put(hbox_icons, (ctx.size.width - (3 * (50 + 10))) / 2, cardIconY);
    fixed.put(button_Quit, (ctx.size.width - 100) / 2, ctx.size.height - 50);
    fixed.put(commonGameElements->puocViews,
              (ctx.size.width - (3 * (155 + 10))) / 2, cardIconY + 50 + 10);
    fixed.put(commonGameElements->tcViews,
              (ctx.size.width - (3 * (155 + 10))) / 2, cardIconY + 50 + 10);
    fixed.put(commonGameElements->procViews, (ctx.size.width - (155 + 10)) / 2,
              cardIconY + 50 + 10);
    fixed.put(roundNumberView,
              (ctx.size.width - v_constants::roundNumberSize.width) / 2, 0);
}

void GamePlayingPageContext::construct_scoreboard(eval_state_c&& scores)
{
    scoreBoard = std::make_unique<ScoreBoard>(SizeInPixels {400, 260},
                                              std::move(scores));
    fixed.put(*scoreBoard, (ctx.size.width - 400) / 2, 10);
    scoreBoard->show();
}