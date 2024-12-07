#ifndef PLAYER_INFORMATION_H
#define PLAYER_INFORMATION_H

#include <iostream>

#include "Board.h"
#include "PrivateObjectiveCard.h"
#include "Typedefs.h"

class EvalState;
class GameFlowController;
class GamePlayingPage;
class SagradaTest;
class Tournament;
class AI_Player;

/**
 * @brief Represents the information that the game holds about a player
 */
struct PlayerInformation
{
    // PlayerInformation(){}

    PlayerInformation(ID_t _ID, std::string _name, proc_t& secretCard)
        : ID(_ID), name(std::move(_name)), PrOC(secretCard)
    {
    }

    PlayerInformation(const PlayerInformation& pi, DiceSupply& newDS,
                      DiceSupply& originalDS)
        : favorTokensLeft(pi.favorTokensLeft)
        , ID(pi.ID)
        , board(pi.board->clone(newDS, originalDS))
        , name(pi.name)
        , PrOC(pi.PrOC)
    {
    }

    auto clone(DiceSupply& newDS, DiceSupply& originalDS)
    {
        return std::make_unique<PlayerInformation>(*this, newDS, originalDS);
    }

    /** Attributes */
    size_t favorTokensLeft;
    ID_t ID;
    board_t board;
    std::string name;
    proc_t PrOC;

    /** Debug */
    friend std::ostream& operator<<(std::ostream& os, PlayerInformation& obj)
    {
        return os << "{Name: " << obj.name << ", ID: " << obj.ID
                  << ", PrOC: " << obj.PrOC << "}";
    }
};

#endif // PLAYER_INFORMATION_H
