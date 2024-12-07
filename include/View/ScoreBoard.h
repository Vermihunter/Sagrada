#ifndef SCORE_BOARD_H
#define SCORE_BOARD_H

#include <gtkmm.h>

#include "EvalState.h"
#include "SizeInPixels.h"
#include "Typedefs.h"

class ScoreBoard : public Gtk::DrawingArea
{
    static const std::string title;

  public:
    ScoreBoard(const SizeInPixels& _size, eval_state_c&& _scores);

  protected:
    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

  private:
    SizeInPixels size;
    eval_state_c scores;

    void draw_perimeter(const Cairo::RefPtr<Cairo::Context>& cr,
                        const Color& color, size_t param, size_t lineWidth,
                        size_t ind);
};

#endif // SCORE_BOARD_H