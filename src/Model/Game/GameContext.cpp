#include "GameContext.h"

GameContext::GameContext(const GameContext& other, rnd_t _rnd)
    : selectableWPC(other.selectableWPC)
    , numberOfRounds(other.numberOfRounds)
    , dicePerRound(other.dicePerRound)
    , playerWpcChoiceCount(other.playerWpcChoiceCount)
    , rnd(std::move(_rnd))
    , supply(other.supply->clone(*rnd))
    , roundTrack(other.roundTrack, *supply, *other.supply)
    , scoreCtx(other.scoreCtx->clone())
    , basicMoveHandler(other.basicMoveHandler->clone())
{
    for (auto&& puoc : other.publicObjectiveCards) {
        publicObjectiveCards.push_back(puoc->clone());
    }

    for (auto&& tc : other.toolCards) {
        toolCards.push_back(tc->clone());
    }

    for (auto&& player : other.players) {
        players.push_back(player->clone(*supply, *other.supply));
    }
}