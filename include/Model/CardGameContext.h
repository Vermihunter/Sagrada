#ifndef CARD_GAME_CONTEXT_H
#define CARD_GAME_CONTEXT_H

/**
 * @brief Unified implementation of Tool card and Public objective card game
 * contexts
 *
 * @tparam T ToolCard or PublicObjectiveCard
 */
template <typename T>
struct CardGameContext
{
    virtual ~CardGameContext() = default;

    size_t cards_per_game() const { return cardsPerGame; }
    virtual size_t selected_card_count() = 0;
    virtual T get_card_type_at_ind(size_t ind) = 0;
    virtual bool contains(T type) = 0;

  protected:
    CardGameContext(size_t _cardsPerGame) : cardsPerGame(_cardsPerGame) {}

    size_t cardsPerGame;
};

#endif // CARD_GAME_CONTEXT_H