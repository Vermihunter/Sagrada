#ifndef DIGITS_H
#define DIGITS_H

#include "Logger.h"
#include "Point.h"

#include <gtkmm/drawingarea.h>

class DrawingCommand {
  public:
	virtual ~DrawingCommand() = default;

	// Made empty implementation because cannot store
	// abstract class instances into the vector otherwise
	virtual void execute(const Cairo::RefPtr<Cairo::Context>& cr, const Point& topLeft) const
	{
		throw std::invalid_argument{"This should be a base class"};
	}

  protected:
	DrawingCommand(const Point& _relativePos) : relativePos(_relativePos) {}

	const Point relativePos;
};

class MoveToCommand : public DrawingCommand {
  public:
	MoveToCommand(const Point& relativePos) : DrawingCommand(relativePos) {}

	virtual void execute(const Cairo::RefPtr<Cairo::Context>& cr,
	                     const Point&                         topLeft) const override
	{
		cr->move_to(relativePos.x + topLeft.x, relativePos.y + topLeft.y);
	}
};

class LineToCommand : public DrawingCommand {
  public:
	LineToCommand(const Point& relativePos) : DrawingCommand(relativePos) {}

	virtual void execute(const Cairo::RefPtr<Cairo::Context>& cr,
	                     const Point&                         topLeft) const override
	{
		cr->line_to(relativePos.x + topLeft.x, relativePos.y + topLeft.y);
	}
};

constexpr size_t blockSize = 10;

using drawing_command_t = std::unique_ptr<DrawingCommand>;
using drawing_command_c = std::vector<drawing_command_t>;

class DigitInfo {
  public:
	static DigitInfo& get()
	{
		static DigitInfo instance;
		return instance;
	}

	const drawing_command_c& operator[](size_t i) { return digitDrawingCommands[i]; }

  private:
	std::vector<drawing_command_c> digitDrawingCommands;

	DigitInfo();
};

/**
 * @brief Global functions that draws a given 2D number using lines and arcs
 * The digits are using a 3x5 block system meaning that the width:height ratio
 * equals 3:5. The blocks are defined by blockSize parameter.
 *
 * Drawing each digit is started on the top left corner and continued to the
 * right until it reaches the starting position.
 *
 * @param cr context to draw with
 * @param digit the digit to draw
 * @param topleftX top left x coordinate of the number
 * @param topleftY top left y coordinate of the number
 */
inline void draw_digit(const Cairo::RefPtr<Cairo::Context>& cr, unsigned int digit,
                       const Point& topLeft)
{
	if (digit >= 10) {
		LOG_E("Undefined digit drawing request: " + std::to_string(digit));
		return;
	}

	cr->set_line_width(2);
	cr->set_source_rgba(0, 0, 0, 1);

	for (auto&& drawingCommand : DigitInfo::get()[digit]) {
		drawingCommand->execute(cr, topLeft);
	}

	cr->stroke();
}

#endif // DIGITS_H
