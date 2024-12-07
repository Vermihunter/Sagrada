#ifndef GAME_PLAYING_PAGE_STATE_H
#define GAME_PLAYING_PAGE_STATE_H

#include "BlockedUserInterraction.h"
#include "Die.h"
#include "GamePlayingPageContext.h"
#include "ToolCardView.h"

struct GamePlayingPageState
{
  private:
    auto _find_die_view(Die* die)
    {
        return std::find_if(ctx.currRoundsDice.diceView.begin(),
                            ctx.currRoundsDice.diceView.end(),
                            [&](const auto& dieView) {
                                return dieView->get_field().get_die() == die;
                            });
    }

    void _unselect_die(Die* die)
    {
        auto dieViewIt = _find_die_view(die);
        if (dieViewIt != ctx.currRoundsDice.diceView.end()) {
            (*dieViewIt)->unselect();
        }
    }

    void _select_die(Die* die)
    {
        auto dieViewIt = _find_die_view(die);
        (*dieViewIt)->set_selected();
        selectedDie = die;
    }

  public:
    GamePlayingPageState(GamePlayingPageContext& _ctx,
                         const BlockedUserInterraction& startingState)
        : selectedDie(nullptr)
        , selectedTc(nullptr)
        , blockState(startingState)
        , ctx(_ctx)
    {
    }

    void select_die(Die* die)
    {
        if (selectedDie == die) {
            return;
        }

        if (selectedDie) {
            _unselect_die(selectedDie);
        }

        _select_die(die);

        ctx.currRoundsDice.queue_draw();
    }

    void unselect_selected_die()
    {
        if (!selectedDie) {
            return;
        }

        _unselect_die(selectedDie);
        ctx.currRoundsDice.queue_draw();
        selectedDie = nullptr;
    }

    void select_tc(ToolCardView* tc)
    {
        // There is a selected Tool card that cannot be changed
        if (selectedTc && !selectedTc->set_selected(false)) {
            return;
        }

        if (selectedTc) {
            selectedTc->reset();
        }

        if (tc && tc != selectedTc) {
            // Saving it only if it wants to be saved (no reason to save tool
            // cards that consists of only a single move and are instantly sent)
            selectedTc = tc->set_selected(true) ? tc : nullptr;
        }
        else {
            selectedTc = nullptr;
        }

        ctx.commonGameElements->tcViews.connect_tool_card(selectedTc);
    }

    void unselect_tc() { select_tc(nullptr); }

    Die* selectedDie;
    ToolCardView* selectedTc;
    BlockedUserInterraction blockState;

  private:
    GamePlayingPageContext& ctx;
};

#endif // GAME_PLAYING_PAGE_STATE_H