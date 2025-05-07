#ifndef ROUND_NUMBER_VIEW_H
#define ROUND_NUMBER_VIEW_H

#define _USE_MATH_DEFINES

#include "Digits.h"
#include "SizeInPixels.h"

#include <cmath>
#include <gtkmm/drawingarea.h>

/**
 * @brief Displays the current round's number
 *
 */
class RoundNumberView : public Gtk::DrawingArea {
  public:
	/** Default constructor */
	RoundNumberView(const size_t& roundNumber, const SizeInPixels& size);

	/** Default destructor */
	virtual ~RoundNumberView();

  protected:
	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);

  private:
	const size_t& roundNumber;
	SizeInPixels  size;

	/** Config parameters */
	double x;
	double y;

	double aspect;
	double corner_radius;

	double radius;
	double degrees;

	double upperPartHeight;
	double lowerPartHeight;

	unsigned int digitBlockSize;

	void draw_calendar_background(const Cairo::RefPtr<Cairo::Context>& cr);
	void draw_round_digit(const Cairo::RefPtr<Cairo::Context>& cr);
};

#endif // ROUND_NUMBER_VIEW_H
