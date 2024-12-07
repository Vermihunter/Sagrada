#ifndef TOOL_CARD_H
#define TOOL_CARD_H

#include <memory>

#include "Constants.h"
#include "Die.h"
#include "GameInfo.h"
#include "ToolCardMove.h"
#include "ToolCardType.h"
#include "ToolCardUseFeedback.h"
#include "Typedefs.h"

/**
 * @brief Gathers together all information that would be passed separately
 * during tool card move appending
 */
struct ToolCardAppendContext
{
    ToolCardAppendContext(move_c& _mc, Game& _g, size_t tcInd, ID_t pId,
                          tool_card_move_t _prevMove)
        : moveContainer(_mc)
        , g(_g)
        , toolCardInd(tcInd)
        , playerId(pId)
        , prevMove(_prevMove)
    {
    }

    move_c& moveContainer;
    Game& g;
    size_t toolCardInd;
    ID_t playerId;
    tool_card_move_t prevMove;
};

class ToolCard
{
  public:
    ToolCard() : useCount(0) {}

    virtual ~ToolCard() = default;

    /**
     * @brief Returns the number of favor tokens that the tool card requires in
     * its current state Virtuality gives the option to concrete Tool cards to
     * override this method.
     */
    virtual const unsigned int& favor_tokens_to_use()
    {
        cost = (useCount == 0) ? 1 : 2;
        return cost;
    }

    /**
     * @brief Appends all moves for a given tool card and it's current state
     * Some tool cards consist of multiple sub-moves, for that reason the
     * ToolCardAppendContext contains the previous sub-move that is continued if
     * its not empty.
     * @param ctx
     */
    virtual void append_possible_moves(ToolCardAppendContext& ctx) = 0;

    /**
     * @brief Defines the functionalities that each tool cards have when used
     *
     * Base virtual version currently used for incrementing useCount
     *
     */
    virtual ToolCardUseFeedback use(tool_card_move_t m, Game& g) = 0;

    /**
     * @brief Performs undo operation on the last sub-move
     * Currently unused
     *
     * @param m Tool card sub-move
     * @param g Game object that is required for undoing the basic moves
     * @return true If all the submoves are undone
     * @return false If there are sub-moves left for the same playerIndex to be
     * undone
     */
    virtual ToolCardUseFeedback undo(tool_card_move_t& m, Game& g) = 0;

    /**
     * @brief Virtual cloning method for tool cards
     *
     * @return tc_t The cloned tool card
     */
    virtual tc_t clone() = 0;

    /**
     * @brief Get the enumeration type of the tool card
     *
     * @return tc_etype Enumerated type of the tool card
     */
    tc_etype get_type() { return etype; }

    /**
     * @brief Factory method to construct a tool card by its enumerated type
     *
     * @param type Enum type
     * @return tc_t Newly constructed tool card
     */
    static tc_t create(tc_etype type);

  protected:
    unsigned int cost;
    unsigned int useCount;
    tc_etype etype;

    /**
     * @brief Simple dynamic cast for a type and hard fail on failure
     *
     * @tparam Derived The underlying type of the move m. Must be a ToolCardMove
     * @param m The move type to be casted
     * @return Derived* The casted tool card move
     */
    template <typename Derived>
        requires std::derived_from<Derived, ToolCardMove>
    static Derived* to_concrete_type(tool_card_move_t& m)
    {
        auto concreteType = dynamic_cast<Derived*>(m);

        if (!concreteType) {
            throw std::runtime_error {
                "Invalid tool card move type passed to use!"};
        }

        return concreteType;
    }

    void base_use_impl();
    void base_undo_impl();

    /**
     * @brief Helper to append all placing moves that is sub-move of multiple
     * Tool cards
     *
     * @param ctx Appending context
     * @param die Die to be placed
     */
    static void append_placing_moves(ToolCardAppendContext& ctx, Die* die);
};

#endif
