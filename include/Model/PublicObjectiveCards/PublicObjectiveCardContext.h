#ifndef PUBLIC_OBJECTIVE_CARD_CONTEXT_H
#define PUBLIC_OBJECTIVE_CARD_CONTEXT_H

#include <memory>

/**
 * @brief Context object for Public objective cards
 *
 */
struct PublicObjectiveCardContext
{
    PublicObjectiveCardContext(int _satisfactionValue)
        : satisfactionValue(_satisfactionValue)
    {
    }

    auto clone() { return std::make_unique<PublicObjectiveCardContext>(*this); }

    const int get_satisfaction_value() const { return satisfactionValue; }

  private:
    int satisfactionValue;
};

using concrete_puoc_context_t = std::unique_ptr<PublicObjectiveCardContext>;

#endif // PUBLIC_OBJECTIVE_CARD_CONTEXT_H