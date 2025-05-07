#ifndef ROUND_TRACK_VIEW_H
#define ROUND_TRACK_VIEW_H

#define _USE_MATH_DEFINES

#include "BoardFieldView.h"
#include "IRoundTrackClickSignallable.h"
#include "ISignaller.h"
#include "RandomGenerator.h"
#include "RoundTrack.h"
#include "SizeInPixels.h"

#include <cmath>
#include <gtkmm.h>
#include <vector>

/**
 * @brief Displays the round track and predefined amount of dice on it
 *
 * If there are more dice on the round track than it could display
 * at a time, clicking the left or right side of it serves as a slider.
 *
 */
class RoundTrackView : public Gtk::DrawingArea, public ISignaller<IRoundTrackClickSignallable> {
  public:
	RoundTrackView(const SizeInPixels& fieldSize, const std::vector<Die*>& _roundTrackDice,
	               size_t displayedBoardFieldCount);

	/** Default destructor */
	virtual ~RoundTrackView() {}
	const SizeInPixels& get_size() { return size; }

  protected:
	/** Drawing area visualizating virtual methods */
	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
	virtual bool on_button_press_event(GdkEventButton* event);

  private:
	SizeInPixels             size;
	size_t                   firstDieIndex;
	const std::vector<Die*>& roundTrackDice;
	size_t                   displayedBoardFieldCount;

	/**
	 * Always only displayedBoardFieldCount fields - using the upper and lower
	 * side could be slided
	 */
	std::vector<std::unique_ptr<BoardFieldView>> roundTrackFields;
	std::vector<BoardField>                      underlyingFields;

	size_t corner_radius;
	double aspect;
	double radius;
	double degrees;
	size_t segmentSize;
	double fieldWithSegmentHeight;

	void signal_all(Die* die);
};

#endif // ROUND_TRACK_VIEW_H
