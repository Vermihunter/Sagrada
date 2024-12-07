#include "RelocateExactlyTwoDiceTC.h"

#include "Game.h"
#include "RelocateDieMoveReferenceFiller.h"

RelocateExactlyTwoDiceTC::RelocateExactlyTwoDiceTC()
{
    // ctor
}

ToolCardUseFeedback RelocateExactlyTwoDiceTC::use(tool_card_move_t m, Game& g)
{
    auto mv = to_concrete_type<RelocateTwoDiceMove>(m);

    if (!mv->hasSecondRelocateMove) {
        LOG_D("BadMoveRequest 15");
        throw BadMoveRequest {"You have to move\nexactly two dice!"};
    }

    check_indices_correct_hardfail(mv->m1, mv->m2);

    /** The dice are relocated only if both the moves are correct */
    check_relocate_move_correct_hardfail(&mv->m1, g);
    check_relocate_move_correct_hardfail(&mv->m2, g);

    use_two_relocate_moves(mv, g);

    base_use_impl();
    return ToolCardUseFeedback {
        .shiftRound = false, .tcUsageFinished = true, .extractCost = true};
}

void RelocateExactlyTwoDiceTC::append_possible_moves(ToolCardAppendContext& ctx)
{
    std::vector<RelocateDieMove> relocMoves;
    append_all_relocate_moves<RelocateDieMove, RelocateDieMoveReferenceFiller>(
        relocMoves, ctx);

    DieToFieldMove dummyDtfm {ctx.playerId, nullptr, BoardIndex {},
                              DTFM_MODE_DEFAULT};
    auto& board = *GameInfo::get_player_with_id(ctx.g, ctx.playerId)->board;
    for (auto&& relocMove1 : relocMoves) {
        // The get_all_relocate_moves() already checks this, so this is
        // certainly possible
        Die* move1Die = board[relocMove1.from]->remove_die();
        dummyDtfm.die = move1Die;
        dummyDtfm.boardIndices = relocMove1.to;
        ctx.g.move_request(&dummyDtfm, true);
        for (auto&& relocMove2 : relocMoves) {
            append_two_relocate_move_if_correct(ctx, relocMove1, relocMove2);
        }

        undo_relocate_move(&relocMove1, ctx.g);
    }
}

ToolCardUseFeedback RelocateExactlyTwoDiceTC::undo(tool_card_move_t& m, Game& g)
{
    auto relocMoves = to_concrete_type<RelocateTwoDiceMove>(m);

    undo_relocate_move(&relocMoves->m1, g);
    undo_relocate_move(&relocMoves->m2, g);

    base_undo_impl();
    return ToolCardUseFeedback {
        .shiftRound = false, .tcUsageFinished = true, .extractCost = true};
}

RelocateExactlyTwoDiceTC::~RelocateExactlyTwoDiceTC()
{
    // dtor
}
