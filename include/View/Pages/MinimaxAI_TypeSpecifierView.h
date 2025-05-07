#ifndef MINIMAX_AI_TYPE_SPECIFIER_VIEW_H
#define MINIMAX_AI_TYPE_SPECIFIER_VIEW_H

#include "AI_TypeSpecifierView.h"
#include "HeuristicConstants.h"
#include "LabeledWidget.h"
#include "MinimaxPlayerConfig.h"

#include <string>

class MinimaxAI_TypeSpecifierView : public AI_TypeSpecifierView {
  public:
	MinimaxAI_TypeSpecifierView() : depthSpecifier("Minimax search depth")
	{
		constexpr int maxSpecifiedDepths = 5;
		for (int i = 1; i <= maxSpecifiedDepths; ++i) {
			depthSpecifier.widget.append(std::to_string(i));
		}
		depthSpecifier.widget.set_active(3);

		pack_start(depthSpecifier);
	}

	virtual ai_player_config_t create_player() override
	{
		int selectedDepth = depthSpecifier.widget.get_active_row_number() + 1;
		return std::make_unique<MinimaxPlayerConfig>(selectedDepth, defaultConstants,
		                                             MINIMAX_defaultWorldCount);
	}

  private:
	LabeledWidget<Gtk::ComboBoxText, false> depthSpecifier;
};

#endif // MINIMAX_AI_TYPE_SPECIFIER_VIEW_H