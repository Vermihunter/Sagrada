#include "ScoreBoard.h"

#include "PlayerInformation.h"

const std::string ScoreBoard::title = "Score board";

ScoreBoard::ScoreBoard(const SizeInPixels& _size, eval_state_c&& _scores)
    : size(_size), scores(std::move(_scores))
{
	set_size_request(size.width, size.height);
}

void ScoreBoard::draw_perimeter(const Cairo::RefPtr<Cairo::Context>& cr, const Color& color,
                                size_t param, size_t lineWidth, size_t ind)
{
	cr->set_source_rgb(color.r, color.g, color.b);

	size_t positionalRelativePos = (ind + 1) * lineWidth;
	cr->move_to(positionalRelativePos, param);
	cr->line_to(param, positionalRelativePos);
	cr->line_to(size.width - param, positionalRelativePos);
	cr->line_to(size.width - positionalRelativePos, param);
	cr->line_to(size.width - positionalRelativePos, size.height - param);
	cr->line_to(size.width - param, size.height - positionalRelativePos);
	cr->line_to(param, size.height - positionalRelativePos);
	cr->line_to(positionalRelativePos, size.height - param);
	cr->line_to(positionalRelativePos, param);
}

bool ScoreBoard::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	const static std::vector<Color*> perimeterColors{
	    &red, &green, &blue, &yellow, &purple // &black,
	};

	constexpr size_t lineWidth = 4;
	cr->set_line_width(lineWidth);

	constexpr size_t startPos = 30;

	cr->set_source_rgb(0, 0, 0);
	draw_perimeter(cr, black, startPos, lineWidth, 0);
	cr->fill();

	size_t ind = 0;
	for (auto&& color : perimeterColors) {
		draw_perimeter(cr, *color, startPos + (ind * lineWidth), lineWidth, ind);
		cr->stroke();
		++ind;
	}

	constexpr double const titleRatio = 0.25;

	/** White title */
	cr->set_source_rgb(1, 1, 1);
	Cairo::TextExtents te;

	cr->select_font_face("Georgia", Cairo::FontSlant::FONT_SLANT_NORMAL,
	                     Cairo::FontWeight::FONT_WEIGHT_BOLD);
	cr->set_font_size(17.5);

	cr->get_text_extents(title, te);

	/** Center align */
	cr->move_to((size.width) / 2 - te.x_bearing - te.width / 2,
	            (size.height * titleRatio) / 2 - te.y_bearing - te.height / 2);

	cr->show_text(title);

	ind = 0;
	for (auto&& score : scores) {
		const auto&  player          = score->get_player();
		const size_t playerPlacement = ind + 1;
		const int    totalPoints     = score->get_total_points();

		const std::string allInfo = std::to_string(playerPlacement) + " - " + player->name + " - " +
		                            std::to_string(totalPoints);
		Cairo::TextExtents te;

		cr->select_font_face("Georgia", Cairo::FontSlant::FONT_SLANT_NORMAL,
		                     Cairo::FontWeight::FONT_WEIGHT_BOLD);
		cr->set_font_size(17.5);

		cr->get_text_extents(allInfo, te);

		/** Center align */
		cr->move_to((size.width) / 2 - te.x_bearing - te.width / 2,
		            size.height * titleRatio + 40 + 35 * ind - te.y_bearing - te.height / 2);

		cr->show_text(allInfo);
		++ind;
	}

	return true;
}
