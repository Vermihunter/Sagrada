#include "RoundTrackView.h"

RoundTrackView::RoundTrackView(const SizeInPixels& fieldSize,
                               const std::vector<Die*>& _roundTrackDice,
                               size_t _displayedBoardFieldCount)
    : firstDieIndex(0)
    , roundTrackDice(_roundTrackDice)
    , displayedBoardFieldCount(_displayedBoardFieldCount)
{
    segmentSize = 5;
    size.height = 2 * segmentSize + fieldSize.height;

    /** Basic configuration that is used each time the round track is drawn*/
    corner_radius = size.height / 2.0;
    aspect = 1.0;
    radius = corner_radius / aspect;
    degrees = M_PI / 180.0;

    size.width = displayedBoardFieldCount * (fieldSize.width + segmentSize) +
                 2 * radius + segmentSize;
    set_size_request(size.width, size.height);

    underlyingFields.reserve(displayedBoardFieldCount);
    for (size_t i = 0; i < displayedBoardFieldCount; ++i) {
        underlyingFields.push_back(BoardField {});
        roundTrackFields.push_back(std::make_unique<BoardFieldView>(
            Point {.x = size_t(radius + segmentSize +
                               i * (fieldSize.width + segmentSize)),
                   .y = segmentSize},
            fieldSize, underlyingFields[i]));
    }

    add_events(Gdk::BUTTON_PRESS_MASK);
}

bool RoundTrackView::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    cr->set_source_rgb(0.0, 0.0, 0.0);

    /** Black base */
    cr->begin_new_sub_path();
    cr->arc(size.width - radius, radius, radius, -90 * degrees, 0 * degrees);
    cr->arc(size.width - radius, size.height - radius, radius, 0 * degrees,
            90 * degrees);
    cr->arc(radius, size.height - radius, radius, 90 * degrees, 180 * degrees);
    cr->arc(radius, radius, radius, 180 * degrees, 270 * degrees);
    cr->close_path();
    cr->fill();

    cr->set_line_width(3);

    for (size_t i = 0; i < displayedBoardFieldCount; ++i) {
        // Actualizing the view according to the current state of the actual
        // round track
        if (i < roundTrackDice.size()) {
            underlyingFields[i].put_die(roundTrackDice[i]);
        }
        else {
            underlyingFields[i].remove_die();
        }

        roundTrackFields[i]->draw(cr);
    }

    /** Pager config */
    cr->set_source_rgba(1.0, 1.0, 1.0, 1.0);
    cr->set_line_width(5);

    /** Left pager */
    cr->begin_new_sub_path();
    cr->move_to(radius, segmentSize);
    cr->line_to(segmentSize, size.height / 2);
    cr->line_to(radius, size.height - segmentSize);
    cr->close_path();

    /** Right pager */
    cr->begin_new_sub_path();
    cr->move_to(size.width - radius, segmentSize);
    cr->line_to(size.width - segmentSize, size.height / 2);
    cr->line_to(size.width - radius, size.height - segmentSize);
    cr->close_path();
    cr->fill();

    cr->set_source_rgba(0.0, 0.0, 0.0, 1.0);

    return true;
}

bool RoundTrackView::on_button_press_event(GdkEventButton* event)
{
    std::cout << "RoundTrackView::on_button_press_event: [" << event->x << ","
              << event->y << "]" << std::endl;
    auto x = event->x;

    /** Left pager hit */
    if (x <= radius) {
        std::cout << "Left pager hit" << std::endl;
        if (firstDieIndex > 0) {
            --firstDieIndex;
            queue_draw();
        }
    }

    /** Right pager hit */
    else if (x >= size.width - radius) {
        std::cout << "Right pager hit" << std::endl;
        if (firstDieIndex + displayedBoardFieldCount < roundTrackDice.size()) {
            ++firstDieIndex;
            queue_draw();
        }
    }

    /** field hit */
    else {
        std::cout << "Field hit check" << std::endl;
        for (auto&& field : roundTrackFields) {
            Die* fieldDie = field->get_field().get_die();
            if (fieldDie && field->contains_point(
                                Point {size_t(event->x), size_t(event->y)})) {
                signal_all(fieldDie);
                break;
            }
        }
    }

    return true;
}

void RoundTrackView::signal_all(Die* die)
{
    std::cout << "RoundTrack die hit: " << die << std::endl;
    for (auto&& observer : observers) {
        observer->receive_round_track_die_click_signal(die);
    }
}