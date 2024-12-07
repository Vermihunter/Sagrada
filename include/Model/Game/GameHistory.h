#ifndef GAME_HISTORY_H
#define GAME_HISTORY_H

#include <list>
#include <memory>
#include <vector>

#include "Constants.h"
#include "IEasyClonable.h"
#include "Move.h"
#include "ToolCardMove.h"

/**
 * @brief Information stored about each turn in the game
 * It must remember the playerId because the tool card MoveTwiceInARowTC may
 * change the order in a way that cannot be reverse-engineered
 */
struct TurnInfo
{
    TurnInfo(ID_t _playerId, bool _tcUsed, size_t _dieOriginalInd = -1)
        : playerId(_playerId), tcUsed(_tcUsed), dieOriginalInd(_dieOriginalInd)
    {
    }

    ID_t playerId;
    bool tcUsed;
    size_t dieOriginalInd;
};

/**
 * @brief Defined for undo operation
 * Includes all the information that cannot be logically deduced from
 * shifting backwards information.
 */
struct RoundInfo : ICloneable<RoundInfo>
{
    std::vector<TurnInfo> turnInfos;
};
using round_info_t = std::unique_ptr<RoundInfo>;

struct MoveInfo
{
    move_t move;
    tool_card_move_t previousToolCardMove;
};

/**
 * @brief Contains the whole history for a game
 * There is a separate stack for moves because in a single round a player could
 * take multiple moves.
 */
struct GameHistory
{
    GameHistory() = default;

    GameHistory(const GameHistory& other, const DiceSupply& from,
                const DiceSupply& to)
    {
        clone_round_history(other.roundsHistory);

        size_t ind = 0;
        MoveInfo* prevInfo = nullptr;
        for (auto&& m : other.moveStack) {
            tool_card_move_t prevToolCardMove = nullptr;
            if (prevInfo && prevInfo->move->t == Move::type::TOOL_CARD) {
                auto tcMoveInfo = ((tool_card_move_t)(prevInfo->move))
                                      ->consists_of_multiple_submoves();
                if (!tcMoveInfo.isSingleMoveToolCard &&
                    tcMoveInfo.followUpMoveExists) {
                    prevToolCardMove = ((tool_card_move_t)(prevInfo->move));
                }
            }

            moveStack.push_back(
                MoveInfo {.move = m.move->clone(from, to),
                          .previousToolCardMove = prevToolCardMove});

            ++ind;
        }
    }

    GameHistory(const GameHistory& other) : moveStack(other.moveStack)
    {
        clone_round_history(other.roundsHistory);
    }

    ~GameHistory()
    {
        // the m.previousToolCardMove doesnt have to be explicitly deleted
        // because it was already deleted in the previous iteration
        for (auto&& m : moveStack) {
            delete m.move;
        }
    }

    std::vector<round_info_t> roundsHistory;
    std::list<MoveInfo> moveStack;

  private:
    void clone_round_history(const std::vector<round_info_t>& otherRoundHistory)
    {
        for (auto&& roundHistory : otherRoundHistory) {
            roundsHistory.push_back(std::make_unique<RoundInfo>(*roundHistory));
        }
    }
};

#endif // GAME_HISTORY_H