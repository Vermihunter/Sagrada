#ifndef I_DIE_CLICK_SIGNALLER_H
#define I_DIE_CLICK_SIGNALLER_H

#include "IDieClickSignallable.h"
#include "ISignaller.h"

/**
 * @brief An additional method for all IDieClickSignallers to make the same
 * calls nicer
 */
struct IDieClickSignaller : ISignaller<IDieClickSignallable>
{
  protected:
    void signal_with_die(Die* die)
    {
        for (auto&& observer : observers) {
            observer->receive_die_click_signal(die);
        }
    }
};

#endif // I_DIE_CLICK_SIGNALLER_H