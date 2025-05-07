#ifndef I_ROUND_TRACK_CLICK_SIGNALLABLE_H
#define I_ROUND_TRACK_CLICK_SIGNALLABLE_H

/**
 * @brief Signallable interface for round track click
 */
struct IRoundTrackClickSignallable {
	virtual ~IRoundTrackClickSignallable()                  = default;
	virtual void receive_round_track_die_click_signal(Die*) = 0;
};

#endif // I_ROUND_TRACK_CLICK_SIGNALLABLE_H