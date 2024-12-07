#ifndef CURRENT_ROUNDS_DICE_VIEW_H
#define CURRENT_ROUNDS_DICE_VIEW_H

#include <gtkmm.h>

#include <vector>

#include "BoardFieldView.h"
#include "IDieClickSignaller.h"
#include "SizeInPixels.h"

class GamePlayingPageState;

class CurrentRoundsDiceView : public Gtk::DrawingArea, public IDieClickSignaller
{
  public:
    constexpr static size_t dicePerRow = 9;

    CurrentRoundsDiceView(const SizeInPixels& _fieldSize,
                          const std::vector<Die*>& _currRoundDice)
        : fieldSize(_fieldSize), currRoundDice(_currRoundDice)
    {
        size_t rowCount = (currRoundDice.size() / dicePerRow) +
                          (currRoundDice.size() % dicePerRow != 0 ? 1 : 0);

        size_t leftAndRightMargin = 8;
        size_t topAndBottomMargin = 8;
        size_t spaceBetweenDice = 10;

        diceWrapper.reserve(currRoundDice.size());
        size_t ind = 0;
        for (auto&& die : currRoundDice) {
            diceWrapper.push_back(BoardField {});
            diceWrapper.back().put_die(die);
            diceView.push_back(std::make_unique<BoardFieldView>(
                Point {.x = leftAndRightMargin +
                            (ind % dicePerRow) *
                                (fieldSize.width + spaceBetweenDice),
                       .y = topAndBottomMargin +
                            (ind / dicePerRow) *
                                (fieldSize.height + spaceBetweenDice)},
                fieldSize, diceWrapper.back()));
            ++ind;
        }

        add_events(Gdk::BUTTON_PRESS_MASK);
        size.width = 2 * leftAndRightMargin +
                     std::min(dicePerRow, currRoundDice.size()) *
                         (fieldSize.width + spaceBetweenDice);
        size.height = 2 * topAndBottomMargin +
                      rowCount * (fieldSize.height + spaceBetweenDice);
        set_size_request(size.width, size.height);
    }

    void initialize_new_round()
    {
        size_t ind = 0;
        for (auto&& die : currRoundDice) {
            diceWrapper[ind].put_die(die);
            ++ind;
        }

        queue_draw();
    }

    const SizeInPixels& get_size() { return size; }

  protected:
    virtual bool on_button_press_event(GdkEventButton* e)
    {
        for (auto&& dieView : diceView) {
            Die* die = dieView->get_field().get_die();
            if (!die) {
                continue;
            }

            const Point& upperLeft = dieView->get_upper_left();
            const SizeInPixels& size = dieView->get_size();

            if (upperLeft.x <= e->x && upperLeft.x + size.width >= e->x &&
                upperLeft.y <= e->y && upperLeft.y + size.height >= e->y) {
                signal_with_die(die);
                break;
            }
        }

        return true;
    }

    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override
    {
        for (auto&& dieWrapper : diceWrapper) {
            auto die = dieWrapper.get_die();
            if (!die) {
                continue;
            }

            auto dieIt =
                std::find(currRoundDice.begin(), currRoundDice.end(), die);
            if (dieIt == currRoundDice.end()) {
                dieWrapper.remove_die();
            }
        }

        for (auto&& dieView : diceView) {
            dieView->draw(cr, false);
        }

        return true;
    }

  private:
    friend GamePlayingPageState;
    SizeInPixels fieldSize;
    SizeInPixels size;
    const std::vector<Die*>& currRoundDice;
    std::vector<std::unique_ptr<BoardFieldView>> diceView;
    std::vector<BoardField> diceWrapper;
};

#endif // CURRENT_ROUNDS_DICE_VIEW_H