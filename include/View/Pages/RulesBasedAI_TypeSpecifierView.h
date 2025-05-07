#ifndef RULES_BASED_AI_TYPE_SPECIFIER_VIEW_H
#define RULES_BASED_AI_TYPE_SPECIFIER_VIEW_H

#include "AI_TypeSpecifierView.h"
#include "LabeledWidget.h"
#include "RulesBasedPlayerConfig.h"

class RulesBasedAI_TypeSpecifierView : public AI_TypeSpecifierView {
  public:
	RulesBasedAI_TypeSpecifierView() : strategyTypes("Strategy")
	{
		strategyTypes.widget.append("only dtfm");
		strategyTypes.widget.append("all moves");
		strategyTypes.widget.set_active(1);

		pack_start(strategyTypes);
	}

	virtual ai_player_config_t create_player() override
	{
		int  strategyIndex = strategyTypes.widget.get_active_row_number();
		auto strategy      = (strategyIndex == 0 ? RulesBasedPlayer::strategy_t::ONLY_DTFM
		                                         : RulesBasedPlayer::strategy_t::ALL_MOVES);
		return std::make_unique<RulesBasedPlayerConfig>(strategy);
	}

  private:
	LabeledWidget<Gtk::ComboBoxText, false> strategyTypes;
};

#endif // RULES_BASED_AI_TYPE_SPECIFIER_VIEW_H