#include "RandomPlayer.h"

#include "Deallocator.h"
#include "Debugger.h"

RandomPlayer::RandomPlayer(const AIPlayerContext& _ctx) : AI_Player(_ctx)
{
    // ctor
}

ID_t RandomPlayer::choose_window_pattern_card(wpc_it_t iBegin, wpc_it_t iEnd)
{
    ID_t choosingOptionCount = iEnd - iBegin;
    return ctx.game.get_ctx()->rnd->random_in_interval(0,
                                                       choosingOptionCount - 1);
}

move_t RandomPlayer::make_next_move()
{
    auto all_moves = ctx.game.possible_moves();

    print_all_moves(all_moves, "Random moves");
    ID_t randomMoveIndex =
        ctx.game.get_ctx()->rnd->random_in_interval(0, all_moves.size() - 1);
    free_all_but(all_moves.begin(), all_moves.end(),
                 all_moves[randomMoveIndex]);

    return all_moves[randomMoveIndex];
}

RandomPlayer::~RandomPlayer()
{
    // dtor
}
