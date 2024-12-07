#ifndef REVERSE_DIE_VALUE_TC_H
#define REVERSE_DIE_VALUE_TC_H

#include "Die.h"
#include "PlacingDieMove.h"
#include "RandomGenerator.h"
#include "ToolCard.h"

/**
 * @brief Implementation of the Grinding Stone Tool card
 *
 * Flips a die from the Draft pool to its opposite value. After the
 * flip, the die has to be placed to the board. If the die with the
 * new value cannot be placed anywhere, the die cannot be flipped.
 */
class ReverseDieValueTC : public ToolCard
{
  public:
    /** Default constructor */
    ReverseDieValueTC();
    /** Default destructor */
    virtual ~ReverseDieValueTC();

    /**
     * Consists of two sub-moves:
     * - reversing a die's value,
     * - placing the same die
     *
     * it assumes that when the move is sent, the die has the
     * original value and this method modifies it
     */
    virtual ToolCardUseFeedback use(tool_card_move_t m, Game& g) override;

    virtual void append_possible_moves(ToolCardAppendContext& ctx) override;

    virtual tc_t clone() override
    {
        return std::make_unique<ReverseDieValueTC>(*this);
    }

    virtual ToolCardUseFeedback undo(tool_card_move_t& m, Game& g) override;

    // View component uses this
    static void reverse_die_value(Die* die);

    using Move = PlacingDieMove;

  private:
    using reverser_move_t = std::shared_ptr<ReverseDieValueTC::Move>;

    void append_moves_for(Die* die, ToolCardAppendContext& ctx);

    void append_value_reverser_moves(move_c& moveContainer, ID_t playerId,
                                     Game& g, ID_t toolCardInd);

    void append_die_placing_moves(move_c& moveContainer,
                                  tool_card_move_t& prevMove);

    void undo_value_reverser_move(reverser_move_t& m);
    bool undo_placing_move(tool_card_move_t& m, Game& g);

    bool move_exists(ID_t playerId, Die* die, Game& g, ID_t toolCardInd);

    move_cache_t cachedMoves;
};

#endif // REVERSE_DIE_VALUE_TC_H
