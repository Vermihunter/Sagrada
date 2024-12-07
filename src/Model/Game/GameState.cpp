#include "GameState.h"

GameState::GameState(const GameState& other, const DiceSupply& from,
                     const DiceSupply& to)
    : hasEnded(other.hasEnded)
    , roundNumber(other.roundNumber)
    , playerStartedThisRoundIndex(other.playerStartedThisRoundIndex)
    , currPlayerIndex(other.currPlayerIndex)
    , previousToolCardMove(nullptr)
    , currRoundInfo(other.currRoundInfo->clone())
{
    for (auto&& die : other.currentRoundRemainingDice) {
        currentRoundRemainingDice.push_back(
            get_corresponding_die(die, from, to));
    }

    if (other.previousToolCardMove) {
        previousToolCardMove = dynamic_cast<ToolCardMove*>(
            other.previousToolCardMove->clone(from, to));
    }
}

GameState::GameState(const GameState& other)
    : hasEnded(other.hasEnded)
    , roundNumber(other.roundNumber)
    , playerStartedThisRoundIndex(other.playerStartedThisRoundIndex)
    , currPlayerIndex(other.currPlayerIndex)
    , previousToolCardMove(other.previousToolCardMove)
    , currentRoundRemainingDice(other.currentRoundRemainingDice)
    , currRoundInfo(other.currRoundInfo->clone())
{
}

ID_t GameState::player_on_move_id()
{
    return currRoundInfo->turnInfos[currPlayerIndex].playerId;
}

bool GameState::is_curr_player_first_move_this_round()
{
    ID_t playerId = player_on_move_id();
    /** Finding the same ID in player order after the current one */
    auto playerIt = std::find_if(
        currRoundInfo->turnInfos.begin() + currPlayerIndex + 1,
        currRoundInfo->turnInfos.end(), [&](const TurnInfo& turnInfo) {
            return turnInfo.playerId == playerId;
        });

    /** If not found, returning false, otherwise true */
    return playerIt != currRoundInfo->turnInfos.end();
}

size_t GameState::moves_remaining_for_player_this_round(ID_t playerId)
{
    return std::count_if(currRoundInfo->turnInfos.begin() + currPlayerIndex,
                         currRoundInfo->turnInfos.end(),
                         [&](const TurnInfo& turnInfo) {
                             return turnInfo.playerId == playerId;
                         });
}