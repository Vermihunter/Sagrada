#ifndef MCTS_AI_TYPE_SPECIFIER_VIEW_H
#define MCTS_AI_TYPE_SPECIFIER_VIEW_H

#include <vector>

#include "AI_TypeSpecifierView.h"
#include "FirstPlayerConfig.h"
#include "HeuristicConstants.h"
#include "LabeledWidget.h"
#include "MCTSPlayerConfig.h"
#include "MinimaxPlayerConfig.h"
#include "RandomPlayerConfig.h"
#include "RulesBasedPlayerConfig.h"

class MCTSAI_TypeSpecifierView : public AI_TypeSpecifierView
{
  public:
    MCTSAI_TypeSpecifierView()
        : iterationCountSpecifier("Iterations")
        , playoutStrategySpecifier("Playout strategy")
        , worldCountSpecifier("World count")
    {
        for (auto&& predefinedIterationCount : predefinedIterationCounts) {
            iterationCountSpecifier.widget.append(
                std::to_string(predefinedIterationCount));
        }
        iterationCountSpecifier.widget.set_active(4);

        playoutStrategySpecifier.widget.append("minimax-depth=1");
        playoutStrategySpecifier.widget.append("rules-based - Only DTFM");
        playoutStrategySpecifier.widget.append("rules-based - All moves");
        playoutStrategySpecifier.widget.append("random");
        playoutStrategySpecifier.widget.append("first");
        playoutStrategySpecifier.widget.set_active(3);

        for (int i = 1; i <= 5; ++i) {
            worldCountSpecifier.widget.append(std::to_string(i));
        }
        worldCountSpecifier.widget.set_active(2);

        pack_start(iterationCountSpecifier);
        pack_start(playoutStrategySpecifier);
        pack_start(worldCountSpecifier);
    }

    virtual ai_player_config_t create_player() override
    {
        ai_player_config_t playoutStrategy;
        mcts_context_t ctx = std::make_unique<MCTSPlayerContext>();
        ctx->iterationCount =
            predefinedIterationCounts[iterationCountSpecifier.widget
                                          .get_active_row_number()];
        ctx->determinizingWorldCount =
            worldCountSpecifier.widget.get_active_row_number() + 1;

        switch (playoutStrategySpecifier.widget.get_active_row_number()) {
            case 0:
                ctx->playoutStrategy = std::make_unique<MinimaxPlayerConfig>(
                    1, defaultConstants, MINIMAX_defaultWorldCount);
                break;
            case 1:
                ctx->playoutStrategy = std::make_unique<RulesBasedPlayerConfig>(
                    RulesBasedPlayer::strategy_t::ONLY_DTFM);
                break;
            case 2:
                ctx->playoutStrategy = std::make_unique<RulesBasedPlayerConfig>(
                    RulesBasedPlayer::strategy_t::ALL_MOVES);
                break;
            case 3:
                ctx->playoutStrategy = std::make_unique<FirstPlayerConfig>();
                break;
            case 4:
                ctx->playoutStrategy = std::make_unique<RandomPlayerConfig>();
                break;
            default:
                break;
        }

        return std::make_unique<MCTSPlayerConfig>(std::move(ctx));
    }

  private:
    inline const static std::vector<int> predefinedIterationCounts {
        10, 20, 30, 50, 100, 150, 200, 250, 300};

    LabeledWidget<Gtk::ComboBoxText, false> iterationCountSpecifier;
    LabeledWidget<Gtk::ComboBoxText, false> playoutStrategySpecifier;
    LabeledWidget<Gtk::ComboBoxText, false> worldCountSpecifier;
};

#endif // MCTS_AI_TYPE_SPECIFIER_VIEW_H
