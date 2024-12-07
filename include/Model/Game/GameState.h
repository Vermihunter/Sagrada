#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "Die.h"
#include "GameHistory.h"
#include "ToolCardMove.h"

/**
 * @brief Represents a given state in the Game
 */
struct GameState
{
    // Default constructor for creating the object
    GameState() : previousToolCardMove(nullptr) {}

    // Copy constructor for cloning
    GameState(const GameState& other, const DiceSupply& from,
              const DiceSupply& to);

    GameState(const GameState& other);

    ~GameState()
    {
        if (previousToolCardMove) {
            // delete previousToolCardMove;
        }
    }

    auto clone(const DiceSupply& from, const DiceSupply& to)
    {
        return std::make_unique<GameState>(*this, from, to);
    }

    auto clone_identical() { return std::make_unique<GameState>(*this); }

    ID_t player_on_move_id();
    bool is_curr_player_first_move_this_round();
    size_t moves_remaining_for_player_this_round(ID_t playerId);

    bool hasEnded;
    size_t roundNumber;
    size_t playerStartedThisRoundIndex;
    size_t currPlayerIndex;

    tool_card_move_t previousToolCardMove;

    std::vector<Die*> currentRoundRemainingDice;
    /** Undo operation */
    round_info_t currRoundInfo;
};

using game_state_t = std::unique_ptr<GameState>;

#endif // GAME_STATE_H