#ifndef PRIVATE_OBJECTIVE_CARD_H
#define PRIVATE_OBJECTIVE_CARD_H

#include <memory>

#include "Attributes.h"
#include "IEasyClonable.h"

/**
 * @brief Represents the secret card of each player
 *
 * The main information about this card is its color,
 * which is hidden and cannot be set only in the constructor.
 */
class PrivateObjectiveCard : ICloneable<PrivateObjectiveCard>
{
  public:
    constexpr static unsigned int totalCardCount =
        (unsigned int)(color_t::COUNT);

    PrivateObjectiveCard(color_t c) : color(c) {}

    /** JSON parsing needs this */
    PrivateObjectiveCard() = default;

    PrivateObjectiveCard(const PrivateObjectiveCard& other)
    {
        this->color = other.color;
    }

    auto get_color() { return color; }

    /**
     * @brief Factory method to create a PrOC having a given color
     *
     * @param t The color that the card will have
     * @return std::unique_ptr<PrivateObjectiveCard> The newly constructed PrOC
     * instance
     */
    static std::unique_ptr<PrivateObjectiveCard> construct(color_t t)
    {
        if (t == color_t::COUNT)
            return nullptr;

        return std::make_unique<PrivateObjectiveCard>(t);
    }

    /** Debug */
    friend std::ostream& operator<<(std::ostream& os,
                                    std::shared_ptr<PrivateObjectiveCard>& proc)
    {
        os << "{Color: " << Color::type_to_string(proc->color) << "}";
        return os;
    }

  private:
    color_t color;
};

using proc_t = std::shared_ptr<PrivateObjectiveCard>;
using proc_c = std::array<proc_t, PrivateObjectiveCard::totalCardCount>;

#endif
