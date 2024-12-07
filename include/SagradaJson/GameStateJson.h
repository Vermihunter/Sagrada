#ifndef GAME_STATE_JSON_H
#define GAME_STATE_JSON_H

#include "DieJson.h"
#include "GameState.h"
#include "MoveJson.h"
#include "RoundInfoJson.h"
#include "SagradaJson.h"

void to_json(json& j, const GameState& gameState)
{
    j = json {
        {"HasEnded", gameState.hasEnded},
        {"RoundNumber", gameState.roundNumber},
        {"PlayerStartedThisRoundIndex", gameState.playerStartedThisRoundIndex},
        {"CurrPlayerIndex", gameState.currPlayerIndex},
        {"CurrentRoundRemainingDice", gameState.currentRoundRemainingDice},
        {"CurrRoundInfo", gameState.currRoundInfo},
        {"PreviousToolCardMove", move_t(gameState.previousToolCardMove)}};
}

#endif // GAME_STATE_JSON_H