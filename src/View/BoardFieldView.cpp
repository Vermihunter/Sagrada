#include "BoardFieldView.h"

BoardFieldView::BoardFieldView(const Point& _upperLeft, const SizeInPixels& _size,
                               BoardField& _boardField)
    : size(_size), upperLeft(_upperLeft), boardField(_boardField), selectedColor(nullptr)
{
}

void BoardFieldView::draw(const Cairo::RefPtr<Cairo::Context>& cr, bool drawEmptyFields)
{
	/** Printing background rectangle with fillColor */
	std::optional<color_t> fillColor;
	if (boardField.get_die()) {
		fillColor = boardField.get_die()->get_color();
	}
	else if (boardField.get_color_restriction().has_value()) {
		fillColor = boardField.get_color_restriction().value();
	}

	std::optional<value_t> val;
	if (boardField.get_die()) {
		val = boardField.get_die()->get_value();
	}
	else if (boardField.get_value_restriction().has_value()) {
		val = boardField.get_value_restriction().value();
	}

	if (!drawEmptyFields && !fillColor.has_value() && !val.has_value()) {
		return;
	}

	auto& color = Color::type_to_rgba(fillColor);

	cr->rectangle(upperLeft.x, upperLeft.y, size.width, size.height);

	if (selectedColor) {
		cr->set_line_width(7);
		cr->set_source_rgba(selectedColor->r, selectedColor->g, selectedColor->b, 1);
		cr->stroke_preserve();
	}

	cr->set_source_rgba(color.r, color.g, color.b, color.a);
	cr->fill();

	Color& circleBackgroundColor = select_circle_background_color(color);

	draw_value_circles(cr, val, circleBackgroundColor);
}

void BoardFieldView::move_relative(const SizeInPixels& relativeMove)
{
	upperLeft.x += relativeMove.width;
	upperLeft.y += relativeMove.height;
}

void BoardFieldView::draw_value_circles(const Cairo::RefPtr<Cairo::Context>& cr,
                                        std::optional<value_t> val, Color& circleBackgroundColor)
{
	if (!val.has_value()) {
		draw_circles_by_value(cr, zeroRadiusMultiplier, circleBackgroundColor);
		return;
	}

	switch (val.value()) {
	case value_t::ONE:
		draw_circles_by_value(cr, oneRadiusMultiplier, circleBackgroundColor);
		break;
	case value_t::TWO:
		draw_circles_by_value(cr, twoRadiusMultiplier, circleBackgroundColor);
		break;
	case value_t::THREE:
		draw_circles_by_value(cr, threeRadiusMultiplier, circleBackgroundColor);
		break;
	case value_t::FOUR:
		draw_circles_by_value(cr, fourRadiusMultiplier, circleBackgroundColor);
		break;
	case value_t::FIVE:
		draw_circles_by_value(cr, fiveRadiusMultiplier, circleBackgroundColor);
		break;
	case value_t::SIX:
		draw_circles_by_value(cr, sixRadiusMultiplier, circleBackgroundColor);
		break;
	case value_t::COUNT:
	default:
		break;
	}
}

template <size_t RadiusMultiplierSize>
void BoardFieldView::draw_circles_by_value(
    const Cairo::RefPtr<Cairo::Context>&                   cr,
    std::array<std::pair<int, int>, RadiusMultiplierSize>& radiusMultiplier,
    Color&                                                 circleBackgroundColor)
{
	constexpr size_t rectangleValueCircleRadiusInPixels = 3;
	Point            middle{upperLeft.x + size.width / 2, upperLeft.y + size.height / 2};

	for (auto&& [xCordRadiusMultiplier, yCordRadiusMultiplier] : radiusMultiplier) {
		draw_circle(cr, middle.x + xCordRadiusMultiplier * rectangleValueCircleRadiusInPixels,
		            middle.y + yCordRadiusMultiplier * rectangleValueCircleRadiusInPixels,
		            rectangleValueCircleRadiusInPixels, circleBackgroundColor);
	}
}

void BoardFieldView::draw_circle(const Cairo::RefPtr<Cairo::Context>& cr, size_t centerXInPixels,
                                 size_t centerYInPixels, size_t radiusInPixels, Color& fill_color)
{
	Color& arcColor = black;

	cr->save();
	cr->set_line_width(6);

	cr->set_source_rgb(double(arcColor.r) / 255.0, double(arcColor.g) / 255.0,
	                   double(arcColor.b) / 255.0);

	cr->arc(centerXInPixels, centerYInPixels, radiusInPixels, 0.0, 2 * M_PI);
	cr->stroke_preserve();

	cr->set_source_rgb(double(fill_color.r), double(fill_color.g), double(fill_color.b));
	cr->fill();

	cr->restore();
}

bool BoardFieldView::contains_point(const Point& point)
{
	return point.x >= upperLeft.x && point.x <= upperLeft.x + size.width &&
	       point.y >= upperLeft.y && point.y <= upperLeft.y + size.height;
}

Color& BoardFieldView::select_circle_background_color(Color& fieldBackgroundColor)
{
	/**
	 * If the background is white, the circles
	 * with white background would not be advantageous.
	 */
	if (&fieldBackgroundColor == &white) {
		return grey;
	}

	return white;
}

BoardFieldView::~BoardFieldView()
{
	// dtor
}

// Zero
std::array<std::pair<int, int>, 0> BoardFieldView::zeroRadiusMultiplier = {};

// One
std::array<std::pair<int, int>, 1> BoardFieldView::oneRadiusMultiplier = {{{0, 0}}};

// Two
std::array<std::pair<int, int>, 2> BoardFieldView::twoRadiusMultiplier = {{{2, 2}, {-2, -2}}};

// Three
std::array<std::pair<int, int>, 3> BoardFieldView::threeRadiusMultiplier = {{
    {2, -2},
    {0, 0},
    {-2, 2},
}};

// Four
std::array<std::pair<int, int>, 4> BoardFieldView::fourRadiusMultiplier = {
    {{-2, -2}, {-2, 2}, {2, -2}, {2, 2}}};

// Five
std::array<std::pair<int, int>, 5> BoardFieldView::fiveRadiusMultiplier = {
    {{-2, -2}, {-2, 2}, {0, 0}, {2, -2}, {2, 2}}};

// Six
std::array<std::pair<int, int>, 6> BoardFieldView::sixRadiusMultiplier = {
    {{-2, -3}, {-2, 3}, {-2, 0}, {2, 0}, {2, -3}, {2, 3}}};
