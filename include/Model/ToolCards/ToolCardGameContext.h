#ifndef TOOL_CARD_GAME_CONTEXT_H
#define TOOL_CARD_GAME_CONTEXT_H

#include <memory>

#include "CardGameContext.h"
#include "ToolCardType.h"

using tc_config_c = std::vector<tc_etype>;

/**
 * @brief Represents the tool card configuration of a game
 *
 */
struct ToolCardGameContext : CardGameContext<tc_etype>
{
    ToolCardGameContext(int _cardsPerGame, const tc_config_c& _selectedCards)
        : CardGameContext(_cardsPerGame), selectedCards(_selectedCards)
    {
    }

    const tc_config_c& get_current_game_tcs() { return selectedCards; }

    size_t selected_card_count() { return selectedCards.size(); }

    tc_etype get_card_type_at_ind(size_t ind)
    {
        if (ind >= selectedCards.size()) {
            throw std::out_of_range {"Index out of range!"};
        }

        return selectedCards[ind];
    }

    virtual bool contains(tc_etype type) override
    {
        auto cardIt =
            std::find(selectedCards.begin(), selectedCards.end(), type);

        return cardIt != selectedCards.end();
    }

  private:
    tc_config_c selectedCards;
};

using tc_context_t = std::unique_ptr<ToolCardGameContext>;

#endif // TOOL_CARD_GAME_CONTEXT_H