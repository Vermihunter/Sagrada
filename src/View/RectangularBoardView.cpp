#include "RectangularBoardView.h"

RectangularBoardView::RectangularBoardView(const BoardContext& _boardCtx, const SizeInPixels& _size,
                                           const PlayerInformation* _playerInfo)
    : boardCtx(_boardCtx), size(_size), playerInfo(_playerInfo), isSelected(false)
{
	fields.resize(boardCtx.config->height);
	for (size_t i = 0; i < boardCtx.config->height; ++i) {
		fields[i].resize(boardCtx.config->width);
	}

	if (playerInfo) {
		size.height += 20;
	}
	set_size_request(size.width, size.height);
	initialize_rectangles();
	add_events(Gdk::BUTTON_PRESS_MASK | Gdk::ENTER_NOTIFY_MASK | Gdk::LEAVE_NOTIFY_MASK);
}

void RectangularBoardView::connect_to_player(const PlayerInformation* _playerInfo)
{
	if (!playerInfo) {
		size.height += 20;
		SizeInPixels relativeFieldMove{0, 20};
		for (auto&& row : fields) {
			for (auto&& field : row) {
				field->move_relative(relativeFieldMove);
			}
		}
	}
	set_size_request(size.width, size.height);
	playerInfo = _playerInfo;
}

void RectangularBoardView::select()
{
	isSelected = true;
	queue_draw();
}

void RectangularBoardView::unselect()
{
	isSelected = false;
	queue_draw();
}

void RectangularBoardView::set_selected(const BoardIndex& ind, Color* color)
{
	(*this)[ind]->set_selected(color);
}

void RectangularBoardView::unselect_all_fields()
{
	for (auto&& row : fields) {
		for (auto&& field : row) {
			field->unselect();
		}
	}
	queue_draw();
}

void RectangularBoardView::select_fields(const board_ind_c& indices, Color* color)
{
	for (auto&& ind : indices) {
		(*this)[ind]->set_selected(color);
	}
	queue_draw();
}

/**
 * @brief Gtk event that is called for drawing
 * Draws the WPC according to its state.
 *
 * @param cr context to draw with
 * @return true always
 * @return false never
 */
bool RectangularBoardView::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	draw_base(cr);
	draw_fields(cr);
	draw_hardness_circles(cr);
	draw_title(cr);

	if (playerInfo) {
		draw_player_name(cr);
	}

	return true;
}

/**
 * @brief Gtk event that is called when the user clicks inside the WPC
 * Handles differently according to the type:
 * - players move
 * - human player choosing WPC
 * - constructing a new WPC
 *
 * @param event event struct that contains the x,y coordinates of the button
 * press
 * @return true always
 * @return false never
 */
bool RectangularBoardView::on_button_press_event(GdkEventButton* event)
{
	handle_attribute_placing_click(event);
	signal_click();

	queue_draw();
	return true;
}

void RectangularBoardView::handle_attribute_placing_click(GdkEventButton* event)
{
	/** The click hit the top of the WPC, where the player's name is shown */
	if (playerInfo) {
		if (event->y <= spaceForPlayerNameInPixels) {
			return;
		}
		event->y -= spaceForPlayerNameInPixels;
	}

	// Either the leftmost or rightmost pixels clicked where are no rectangles,
	// but the margin
	if (event->x < distanceXFromBorderInPixels ||
	    event->x > size.width - distanceXFromBorderInPixels ||
	    event->y < distanceYFromBorderInPixels ||
	    event->y > size.height - distanceYFromBorderInPixels) {
		// Not a rectangle hit -> left or right border hit
		return;
	}

	// Making the upper left rectangle upper left point as 0,0
	event->x -= distanceXFromBorderInPixels;
	event->y -= distanceYFromBorderInPixels;

	// Dividing the "remaining" field for pieces, where one piece is a rectangle
	// and the space until its neighbor
	const size_t onePieceWidth  = rectangleSize.width + distanceBetweenRectanglesInPixels;
	const size_t onePieceHeight = rectangleSize.height + distanceBetweenRectanglesInPixels;

	// Getting the index by dividing by pieces
	size_t colInd = event->x / (onePieceWidth);
	size_t rowInd = event->y / (onePieceHeight);

	// Check if the click hit the space between two rectangles horizontally or
	// the Y coordinate is bigger than the rigthmost column
	if (colInd != size_t((event->x + distanceBetweenRectanglesInPixels) / (onePieceHeight)) ||
	    colInd >= boardCtx.config->width) {
		// Not a rectangle hit -> wrong X coordinate
		return;
	}

	// Check if the click hit the space between two rectangles vertically or
	// lower than the last row of rectangles
	if (rowInd != size_t((event->y + distanceBetweenRectanglesInPixels) / (onePieceHeight)) ||
	    rowInd >= boardCtx.config->height) {
		// Not a rectangle hit -> wrong Y coordinate
		return;
	}

	/** Signal the click to the parent*/
	signal_field_observers(BoardIndex{rowInd, colInd});
}

void RectangularBoardView::initialize_rectangles()
{
	rectangleSize.width = (size.width - 2 * distanceXFromBorderInPixels -
	                       (boardCtx.config->width - 1) * distanceBetweenRectanglesInPixels) /
	                      boardCtx.config->width;

	rectangleSize.height = rectangleSize.width;

	auto additionalHeight = (playerInfo ? spaceForPlayerNameInPixels : 0);

	for (auto&& boardInd : boardCtx.config->type->get_all_indices()) {
		fields[boardInd.row][boardInd.col] = std::make_unique<BoardFieldView>(
		    Point{distanceXFromBorderInPixels + boardInd.col * rectangleSize.width +
		              boardInd.col * distanceBetweenRectanglesInPixels,
		          distanceYFromBorderInPixels + boardInd.row * rectangleSize.height +
		              boardInd.row * distanceBetweenRectanglesInPixels + additionalHeight},
		    rectangleSize, *boardCtx.data[boardInd.row][boardInd.col]);
	}
}

/** Drawing divided into smaller functonalities */
void RectangularBoardView::draw_base(const Cairo::RefPtr<Cairo::Context>& cr)
{
	constexpr size_t lineWidthInPixels = 4;
	constexpr size_t x                 = lineWidthInPixels;
	constexpr size_t y                 = lineWidthInPixels;

	size_t width  = size.width - 2 * lineWidthInPixels;
	size_t height = size.height - 2 * lineWidthInPixels;

	constexpr double aspect = 1.0;

	const double     radius  = (440.0 / 30.0) / aspect;
	constexpr double degrees = M_PI / 180.0;

	cr->set_line_width(2.0);

	if (isSelected) {
		/** Red surrounding */
		cr->set_source_rgb(1.0, 0.0, 0.0);

		cr->begin_new_sub_path();
		cr->arc(x + width - radius, y + radius, radius, -90 * degrees, 0 * degrees);
		cr->arc(x + width - radius, y + height - radius, radius, 0 * degrees, 90 * degrees);
		cr->arc(x + radius, y + height - radius, radius, 90 * degrees, 180 * degrees);
		cr->arc(x + radius, y + radius, radius, 180 * degrees, 270 * degrees);
		cr->close_path();

		cr->fill();

		width -= lineWidthInPixels / 2;
		height -= lineWidthInPixels / 2;
	}

	/** Black base rectangle */
	cr->set_source_rgb(0.0, 0.0, 0.0);
	cr->move_to(0, 0);
	cr->begin_new_sub_path();
	cr->arc(x + width - radius, y + radius + (isSelected ? x / 2 : 0), radius, -90 * degrees,
	        0 * degrees);
	cr->arc(x + width - radius, y + height - radius, radius, 0 * degrees, 90 * degrees);
	cr->arc(x + radius + (isSelected ? x / 2 : 0), y + height - radius, radius, 90 * degrees,
	        180 * degrees);
	cr->arc(x + radius + (isSelected ? x / 2 : 0), y + radius + (isSelected ? x / 2 : 0), radius,
	        180 * degrees, 270 * degrees);
	cr->close_path();

	// SIGSEV happened here
	cr->fill();
}

void RectangularBoardView::draw_fields(const Cairo::RefPtr<Cairo::Context>& cr)
{
	for (auto&& row : fields) {
		for (auto&& field : row) {
			field->draw(cr);
		}
	}
}

void RectangularBoardView::draw_title(const Cairo::RefPtr<Cairo::Context>& cr)
{
	// auto additionalSpace = (playerInfo ? spaceForPlayerNameInPixels : 0);

	/** White title */
	cr->set_source_rgb(1.0, 1.0, 1.0);
	Cairo::TextExtents te;

	cr->select_font_face("Georgia", Cairo::FontSlant::FONT_SLANT_NORMAL,
	                     Cairo::FontWeight::FONT_WEIGHT_BOLD);
	cr->set_font_size(titleFontSizeInPixels);

	cr->get_text_extents(boardCtx.title, te);

	/** Center align */
	cr->move_to((size.width) / 2 - te.x_bearing - te.width / 2,
	            size.height - lowerRowFromBottomInPixels // - spaceForPlayerNameInPixels
	);

	cr->show_text(boardCtx.title);
}

void RectangularBoardView::draw_hardness_circles(const Cairo::RefPtr<Cairo::Context>& cr)
{
	// auto additionalSpace = (playerInfo ? spaceForPlayerNameInPixels : 0);

	constexpr size_t localDistanceXFromBorderInPixels    = 25;
	constexpr size_t localDistanceBetweenCirclesInPixels = 10;
	/** This should be computed from the size of the WPC */
	constexpr size_t radiusInPixels = 4;

	cr->set_source_rgb(1.0, 1.0, 1.0);
	cr->move_to(size.width - localDistanceXFromBorderInPixels,
	            size.height - lowerRowFromBottomInPixels -
	                radiusInPixels //+ additionalSpace //- spaceForPlayerNameInPixels
	);

	/**
	 * If the playersfavorToken is already set, then use the current amount of
	 * player's favor tokens, otherwise use the Window pattern card's hardness
	 */
	auto hardnessCircleCount = (playerInfo ? playerInfo->favorTokensLeft : boardCtx.hardness);
	for (size_t i = 0; i < hardnessCircleCount; ++i) {
		cr->arc(size.width - localDistanceXFromBorderInPixels -
		            i * localDistanceBetweenCirclesInPixels,
		        size.height - lowerRowFromBottomInPixels -
		            radiusInPixels, //+ additionalSpace, // -
		                            // spaceForPlayerNameInPixels,
		        radiusInPixels, 0.0, 2 * M_PI);
	}

	cr->fill();
}

void RectangularBoardView::draw_player_name(const Cairo::RefPtr<Cairo::Context>& cr)
{
	/** White title */
	cr->set_source_rgb(1.0, 1.0, 1.0);
	Cairo::TextExtents te;

	cr->select_font_face("Georgia", Cairo::FontSlant::FONT_SLANT_NORMAL,
	                     Cairo::FontWeight::FONT_WEIGHT_BOLD);
	cr->set_font_size(titleFontSizeInPixels);

	cr->get_text_extents(playerInfo->name, te);

	/** Center align */
	cr->move_to((size.width) / 2 - te.x_bearing - te.width / 2,
	            22 // - spaceForPlayerNameInPixels
	);

	cr->show_text(playerInfo->name);
}

void RectangularBoardView::signal_field_observers(const BoardIndex& ind)
{
	for (auto&& observer : observers) {
		observer->receive_board_field_click_signal(this, ind);
	}
}

void RectangularBoardView::signal_click()
{
	for (auto&& observer : observers) {
		observer->receive_wpc_click_signal(this);
	}
}
