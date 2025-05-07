#ifndef PUBLIC_OBJECTIVE_CARD_GAME_CONTEXT_H
#define PUBLIC_OBJECTIVE_CARD_GAME_CONTEXT_H

#include "CardGameContext.h"
#include "PublicObjectiveCardContext.h"
#include "PublicObjectiveCardType.h"

#include <exception>
#include <memory>
#include <vector>

using puoc_config_c = std::vector<std::pair<puoc_etype, PublicObjectiveCardContext>>;

/**
 * @brief Defines a Public objective card configuration for Game
 *
 */
struct PublicObjectiveCardGameContext : CardGameContext<puoc_etype> {
	PublicObjectiveCardGameContext(int _cardsPerGame, const puoc_config_c& _selectedCards)
	    : CardGameContext(_cardsPerGame), selectedCards(_selectedCards)
	{
	}

	const puoc_config_c& get_current_game_puocs() { return selectedCards; }

	size_t selected_card_count() { return selectedCards.size(); }

	puoc_etype get_card_type_at_ind(size_t ind)
	{
		if (ind >= selectedCards.size()) {
			throw std::out_of_range{"Invalid card ind"};
		}

		return selectedCards[ind].first;
	}

	virtual bool contains(puoc_etype type) override
	{
		auto cardIt =
		    std::find_if(selectedCards.begin(), selectedCards.end(),
		                 [&](const auto& cardConfig) { return type == cardConfig.first; });

		return cardIt != selectedCards.end();
	}

	concrete_puoc_context_t get_context_for(puoc_etype type)
	{
		auto cardIt =
		    std::find_if(selectedCards.begin(), selectedCards.end(),
		                 [&](const auto& cardConfig) { return type == cardConfig.first; });

		if (cardIt == selectedCards.end()) {
			return nullptr;
		}

		return std::make_unique<PublicObjectiveCardContext>(cardIt->second);
	}

  private:
	puoc_config_c selectedCards;
};

using puoc_context_t = std::unique_ptr<PublicObjectiveCardGameContext>;

#endif