#ifndef RELOCATEMAXTWOSAMECOLORDICETCVIEW_HPP
#define RELOCATEMAXTWOSAMECOLORDICETCVIEW_HPP

#include "RelocatingToolCardView.hpp"

class RelocateMaxTwoSameColorDiceTCView : public RelocatingToolCardView
{
  public:
    RelocateMaxTwoSameColorDiceTCView(ToolCardViewContainer& _parentContainer,
                                      Game& game, ToolCard& tc,
                                      const SizeInPixels& size, size_t ind)
        : RelocatingToolCardView(_parentContainer, game, tc, size, ind)
        , size(550, 70)
    {
    }

    virtual void on_common_help_visualizer_draw(
        const Cairo::RefPtr<Cairo::Context>& cr) override
    {
        if (!firstRelocateMove) {
            return;
        }

        const static std::vector<Color*> perimeterColors {
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
            draw_perimeter(cr, *color, startPos + (ind * lineWidth), lineWidth,
                           ind);
            cr->stroke();
            ++ind;
        }

        /** White title */
        cr->set_source_rgb(1, 1, 1);
        Cairo::TextExtents te;

        cr->select_font_face("Georgia", Cairo::FontSlant::FONT_SLANT_NORMAL,
                             Cairo::FontWeight::FONT_WEIGHT_BOLD);
        cr->set_font_size(13);

        cr->get_text_extents("Relocate a single die", te);

        /** Center align */
        cr->move_to((size.width) / 2 - te.x_bearing - te.width / 2,
                    (size.height) / 2 - te.y_bearing - te.height / 2);

        cr->show_text("Relocate a single die");
    }

    virtual void on_common_help_visualizer_click(GdkEventButton* event) override
    {
        if (!firstRelocateMove) {
            return;
        }

        auto move = new RelocateTwoDiceMove(*firstRelocateMove);
        reset();
        signalProcessor->send_move_request(move, moveResultHandlingInfo);
    }

  private:
    SizeInPixels size;

    void draw_perimeter(const Cairo::RefPtr<Cairo::Context>& cr,
                        const Color& color, size_t param, size_t lineWidth,
                        size_t ind)
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
};

#endif // RELOCATEMAXTWOSAMECOLORDICETCVIEW_HPP