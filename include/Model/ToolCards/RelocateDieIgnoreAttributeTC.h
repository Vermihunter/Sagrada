#ifndef RELOCATE_DIE_IGNORE_ATTRIBUTE_TC_H
#define RELOCATE_DIE_IGNORE_ATTRIBUTE_TC_H

#include "Move.h"
#include "RelocatingTC.h"

class RelocateMoveFiller
{
  public:
    static void AddRelocateMove(move_c& dest, ID_t _playerId, size_t _ind,
                                const BoardIndex& _from, const BoardIndex& _to,
                                dtfm_mode_mask_t _modeMask)
    {
        dest.push_back(
            new RelocateDieMove(_playerId, _ind, _from, _to, _modeMask));
    }
};

/**
 * @brief Implementation of the two tool cards below
 *
 * Since both Tool cards have totally the same functionality and implementation
 * depending only on a single compile-time constant (attribute to be ignored),
 * their implementation is defined by this class.
 *
 * Both the enum values have to be used because of the compile-time check for
 * all implementation being available.
 *
 * @tparam _AttributeIgnorer Attribute to be ignored
 */
template <dtfm_mode_t _AttributeIgnorer>
class RelocateDieIgnoreAttributeTC : public RelocatingTC
{
  public:
    static constexpr dtfm_mode_mask_t mode =
        dtfm_mode_mask_t(_AttributeIgnorer);

    static_assert(mode == DTFM_MODE_IGNORE_COLOR ||
                      mode == DTFM_MODE_IGNORE_VALUE,
                  "Incorrect constant, only the above mentioned 2 constants "
                  "could be used!");

    /** Default constructor */
    RelocateDieIgnoreAttributeTC() = default;
    /** Default destructor */
    virtual ~RelocateDieIgnoreAttributeTC() = default;

    virtual ToolCardUseFeedback use(tool_card_move_t m, Game& g) override
    {
        auto relocMove = to_concrete_type<RelocateDieMove>(m);

        check_relocate_move_correct_hardfail(relocMove, g);
        use_relocate_move(relocMove, g);

        base_use_impl();
        return ToolCardUseFeedback {
            .shiftRound = false, .tcUsageFinished = true, .extractCost = true};
    }

    virtual void append_possible_moves(ToolCardAppendContext& ctx) override
    {
        // auto relocMoves = get_all_relocate_moves(ctx, mode);
        // ctx.moveContainer.insert(ctx.moveContainer.end(), relocMoves.begin(),
        // relocMoves.end());
        append_all_relocate_moves<Move*, RelocateMoveFiller>(ctx.moveContainer,
                                                             ctx, mode);
    }

    virtual tc_t clone() override
    {
        return std::make_unique<RelocateDieIgnoreAttributeTC>(*this);
    }

    virtual ToolCardUseFeedback undo(tool_card_move_t& m, Game& g) override
    {
        auto relocMove = to_concrete_type<RelocateDieMove>(m);

        undo_relocate_move(relocMove, g);

        base_undo_impl();
        return ToolCardUseFeedback {
            .shiftRound = false, .tcUsageFinished = true, .extractCost = true};
    }
};

#endif // RELOCATE_DIE_IGNORE_ATTRIBUTE_TC_H
