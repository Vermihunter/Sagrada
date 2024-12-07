#ifndef I_NEW_ROUND_START_SIGNALLABLE_H
#define I_NEW_ROUND_START_SIGNALLABLE_H

/**
 * @brief Represents interface for components that must do some work right after
 * a new round starts/game ends
 */
struct INewRoundStartSignallable
{
    virtual ~INewRoundStartSignallable() = default;
    virtual void signal_new_round_start() = 0;
    virtual void signal_game_end() = 0;
};

#endif // I_NEW_ROUND_START_SIGNALLABLE_H