#ifndef BOARD_FIELD_VIEW_H
#define BOARD_FIELD_VIEW_H

#define _USE_MATH_DEFINES

#include "Attributes.h"
#include "BoardField.h"
#include "Point.h"
#include "SizeInPixels.h"

#include <gtkmm.h>
#include <math.h>

/**
 * @brief Represents a field to display
 *
 * The class only holds the data about its position because
 * it doesn't change. It doesn't store a reference to a board field,
 * because there are instances that are used for visualizing e.g. dice.
 *
 * It is represented as a rectangle having color predefined in the
 * draw function and containing value circles representing the die value
 * if there is any.
 */
class BoardFieldView {
  public:
	/**
	 * @brief Constructs a new Board field view element
	 *
	 * It doesn't take any color or value parameters because they can
	 * change and are searched for each time draw is called.
	 */
	BoardFieldView(const Point& upperLeft, const SizeInPixels& size, BoardField& boardField);

	virtual ~BoardFieldView();

	/**
	 * @brief Moves the top left point by relative distance
	 */
	void move_relative(const SizeInPixels&);

	/** Information getters about the field, some of them probably won't be used
	 */
	const Point& get_upper_left() { return upperLeft; }

	bool contains_point(const Point& point);

	const SizeInPixels& get_size() { return size; }

	/** Used for (un)marking the clicked die */
	void set_selected(Color* strokeColor = &black) { selectedColor = strokeColor; }

	void unselect() { selectedColor = nullptr; }

	void draw(const Cairo::RefPtr<Cairo::Context>& cr, bool drawEmptyFields = true);

	auto& get_field() { return boardField; }

  private:
	SizeInPixels size;
	Point        upperLeft;
	BoardField&  boardField;
	Color*       selectedColor;

	/**
	 * @brief Circles with different fillings could have a different stroke
	 * color Used for drawing white circles with grey stroke so it could be
	 * seen.
	 *
	 * @param fieldBackgroundColor
	 * @return Color&
	 */
	Color& select_circle_background_color(Color& fieldBackgroundColor);

	/**
	 * @brief Part that draws the points to the dice
	 */
	void draw_circle(const Cairo::RefPtr<Cairo::Context>& cr, size_t centerXInPixels,
	                 size_t centerYInPixels, size_t radiusInPixels, Color& fill_color);

	void draw_value_circles(const Cairo::RefPtr<Cairo::Context>& cr, std::optional<value_t> val,
	                        Color& circleBackgroundColor);

	/**
	 * @brief It is a templated function so it could draw all the points with a
	 * single function
	 *
	 * @tparam RadiusMultiplierSize number of points to display
	 * @param cr drawing context
	 * @param radiusMultiplier the predefined positions of the points
	 * @param circleBackgroundColor color of the points
	 */
	template <size_t RadiusMultiplierSize>
	void
	draw_circles_by_value(const Cairo::RefPtr<Cairo::Context>&                   cr,
	                      std::array<std::pair<int, int>, RadiusMultiplierSize>& radiusMultiplier,
	                      Color& circleBackgroundColor);

	static std::array<std::pair<int, int>, 0> zeroRadiusMultiplier;
	static std::array<std::pair<int, int>, 1> oneRadiusMultiplier;
	static std::array<std::pair<int, int>, 2> twoRadiusMultiplier;
	static std::array<std::pair<int, int>, 3> threeRadiusMultiplier;
	static std::array<std::pair<int, int>, 4> fourRadiusMultiplier;
	static std::array<std::pair<int, int>, 5> fiveRadiusMultiplier;
	static std::array<std::pair<int, int>, 6> sixRadiusMultiplier;
};

#endif // BOARD_FIELD_VIEW_H
