#ifndef I_DIE_CLICK_SIGNALLABLE_H
#define I_DIE_CLICK_SIGNALLABLE_H

/**
 * @brief Signallable definition for dice views' observer pattern
 */
struct IDieClickSignallable {
	virtual ~IDieClickSignallable()             = default;
	virtual void receive_die_click_signal(Die*) = 0;
};

#endif // I_DIE_CLICK_SIGNALLABLE_H