#include "RoundNumberView.h"

RoundNumberView::RoundNumberView(const size_t& _roundNumber, const SizeInPixels& _size)
    : roundNumber(_roundNumber), size(_size)
{
	x = 0;
	y = 10, aspect = 1.0;
	corner_radius = size.height / 10.0; /* corner curvature radius */

	radius  = corner_radius / aspect;
	degrees = M_PI / 180.0;

	upperPartHeight = size.height * 0.4;
	lowerPartHeight = size.height - upperPartHeight;

	digitBlockSize = 10;

	set_size_request(size.width, size.height);
}

bool RoundNumberView::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	draw_calendar_background(cr);
	draw_round_digit(cr);
	return true;
}

void RoundNumberView::draw_round_digit(const Cairo::RefPtr<Cairo::Context>& cr)
{
	unsigned int topLeftX = x + size.width / 2 - 1.5 * double(digitBlockSize);
	unsigned int topLeftY = upperPartHeight + lowerPartHeight / 2 - 2.5 * double(digitBlockSize);

	if (roundNumber < 10) {
		draw_digit(cr, roundNumber, Point{topLeftX, topLeftY});
	}
	else {
		draw_digit(cr, roundNumber / 10, Point{size_t(topLeftX - 1.5 * digitBlockSize), topLeftY});
		draw_digit(cr, roundNumber % 10, Point{size_t(topLeftX + 1.5 * digitBlockSize), topLeftY});
	}
}

void RoundNumberView::draw_calendar_background(const Cairo::RefPtr<Cairo::Context>& cr)
{
	/** Lower rectangle begin */
	cr->move_to(size.width, upperPartHeight);
	cr->arc(x + size.width - radius, size.height - radius, radius, 0 * degrees, 90 * degrees);
	cr->arc(x + radius, size.height - radius, radius, 90 * degrees, 180 * degrees);
	cr->line_to(0, upperPartHeight);
	cr->set_source_rgba(1, 1, 1, 1);
	cr->fill_preserve();
	cr->set_source_rgba(0, 0, 0, 1);
	cr->stroke();
	/** Lower rectangle end */

	/** Upper rectangle begin */
	cr->move_to(0, upperPartHeight);
	cr->arc(x + radius, y + radius, radius, 180 * degrees, 270 * degrees);
	cr->arc(x + size.width - radius, y + radius, radius, -90 * degrees, 0 * degrees);
	cr->line_to(size.width, upperPartHeight);
	cr->set_source_rgba(0.5, 0.5, 1, 1);
	cr->fill_preserve();
	cr->set_source_rgba(0, 0, 0, 1);
	cr->stroke();
	/** Upper rectangle end */

	/** Clips begin */
	constexpr unsigned int clipCount = 8;
	constexpr unsigned int margin    = 10;
	constexpr unsigned int rezerva   = 5;

	unsigned int widthForCircle = (size.width - 2 * margin) / clipCount;

	for (unsigned int i = 0; i < clipCount; ++i) {
		/** white background circle*/
		auto localRadius = (widthForCircle - rezerva) / 2;
		cr->arc(i * widthForCircle + widthForCircle / 2 + x + margin, y + localRadius, 3, 0.0,
		        2 * M_PI);
		cr->set_source_rgba(1, 1, 1, 1);
		cr->fill();

		/** black clip */
		cr->set_source_rgba(0, 0, 0, 1);
		cr->arc(i * widthForCircle + widthForCircle / 2 + x + margin, y, localRadius, M_PI,
		        M_PI / 2);
		cr->stroke();
	}
	/** Clips end */

	/** Round title begin */
	cr->set_source_rgb(1.0, 1.0, 1.0);
	Cairo::TextExtents te;

	cr->select_font_face("Georgia", Cairo::FontSlant::FONT_SLANT_NORMAL,
	                     Cairo::FontWeight::FONT_WEIGHT_BOLD);
	cr->set_font_size(20);

	cr->get_text_extents("Round", te);

	cr->move_to((size.width) / 2 - te.x_bearing - te.width / 2 + x, // center align
	            y + upperPartHeight / 2 - (te.y_bearing + te.height / 2) - 3);

	cr->show_text("Round");
	/** Round title end */
}

RoundNumberView::~RoundNumberView()
{
	// dtor
}
