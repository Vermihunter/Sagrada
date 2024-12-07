#ifndef RELOCATING_TC_H
#define RELOCATING_TC_H

#include "DieToFieldMove.h"
#include "PlayerInformation.h"
#include "RelocateTwoDiceMove.h"
#include "ToolCard.h"

enum class relocating_index_check_res_t
{
    IND_OK,
    TWO_SAME_TO,
    TWO_SAME_FROM,
    TO_EQ_FROM
};
enum class relocating_check_res_t
{
    MOVE_OK,
    TO_EQ_FROM,
    NO_DIE,
    DTFM_BAD
};

/**
 * @brief Base class for all relocating moves
 *
 * Contains functionality that is part of each relocating move
 */
class RelocatingTC : public ToolCard
{
    const static std::vector<std::string> indexErrorStringTable;
    const static std::vector<std::string> moveErrorStringTable;

  protected:
    /**
     * @brief Undo for relocate moves
     * This function doesn't send a move request,
     * but moves the die back to its original field
     *
     * Expects the relocMove to be correct
     *
     * @param relocMove
     * @param g
     */
    void undo_relocate_move(relocate_move_t relocMove, Game& g);
    void use_relocate_move(relocate_move_t relocMove, Game& g);
    void use_two_relocate_moves(relocate_two_dice_move_t mv, Game& g);

    bool check_relocate_move_correct_softfail(const BoardIndex& from,
                                              const BoardIndex& to,
                                              ID_t playerId,
                                              dtfm_mode_mask_t relocMoveMode,
                                              Game& g);
    void check_relocate_move_correct_hardfail(relocate_move_t relocMove,
                                              Game& g);

    void append_two_relocate_move_if_correct(ToolCardAppendContext& ctx,
                                             const RelocateDieMove& relocMove1,
                                             const RelocateDieMove& relocMove2);

    bool check_indices_correct_softfail(const RelocateDieMove& mv1,
                                        const RelocateDieMove& mv2);
    void check_indices_correct_hardfail(const RelocateDieMove& mv1,
                                        const RelocateDieMove& mv2);

    template <typename T, typename ContainerFiller>
    void append_all_relocate_moves(std::vector<T>& destContainer,
                                   ToolCardAppendContext& ctx,
                                   dtfm_mode_mask_t mode = DTFM_MODE_DEFAULT)
    {
        auto& board = GameInfo::get_player_with_id(ctx.g, ctx.playerId)->board;

        BoardIndex ind {0, 0};
        for (auto&& row : board->get_ctx()->data) {
            for (auto&& field : row) {
                if (!field->get_die()) {
                    ++ind.col;
                    continue;
                }

                for (auto&& emptyFieldInd : board->get_empty_fields()) {
                    if (!check_relocate_move_correct_softfail(
                            ind, emptyFieldInd, ctx.playerId, mode, ctx.g)) {
                        continue;
                    }
                    ContainerFiller::AddRelocateMove(
                        destContainer, ctx.playerId, ctx.toolCardInd, ind,
                        emptyFieldInd, mode);

                    // destContainer.push_back(new RelocateDieMove(ctx.playerId,
                    // ctx.toolCardInd, ind, emptyFieldInd, mode));
                }

                ++ind.col;
            }
            ind.col = 0;
            ++ind.row;
        }
    }

  private:
    relocating_check_res_t check_relocate_move_correct(
        const BoardIndex& from, const BoardIndex& to, ID_t playerId,
        dtfm_mode_mask_t relocMoveMode, Game& g);

    relocating_index_check_res_t check_indices_correct(
        const RelocateDieMove& mv1, const RelocateDieMove& mv2);
};

#endif // RELOCATING_TC_H