#include <cairomm/cairomm.h>
#include <gtkmm.h>

#include <algorithm>
#include <iostream>
#include <vector>

#include "GameCache.h"
#include "RectangularBoardView.h"
#include "Typedefs.h"
#include "ViewConstants.h"

// Function to draw on the surface, replace with your actual drawing code
void draw_on_surface(const Cairo::RefPtr<Cairo::Context>& cr)
{
    // Example drawing: Draw a red rectangle
    cr->set_source_rgb(1.0, 0.0, 0.0); // Red color
    cr->rectangle(10, 10, 100, 100);   // Rectangle dimensions
    cr->fill();
}

// Function to save DrawingArea content
void save_drawing_area_to_png(std::unique_ptr<RectangularBoardView>& boardView)
{
    // Create an image surface
    auto surface = Cairo::ImageSurface::create(
        Cairo::FORMAT_ARGB32, 400,
        400 / rectangularBoardViewWidthToHeightRatio);

    // Create a Cairo context to draw on the surface
    auto cr = Cairo::Context::create(surface);

    // Draw onto the surface using your drawing function
    boardView->on_draw(cr);

    std::string saveFileName = boardView->get_ctx().title;
    auto newEnd = std::remove(saveFileName.begin(), saveFileName.end(), ' ');

    saveFileName.erase(newEnd, saveFileName.end());

    std::cout << "I am saving " << saveFileName << std::endl;
    saveFileName += ".png";

    // Save the surface to a PNG file
    surface->write_to_png(saveFileName);
}

int main(int argc, char* argv[])
{
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

    std::vector<std::unique_ptr<RectangularBoardView>> wpcViews;
    Gtk::Window window;
    // window.set_default_size(200, 200);

    const int wpcWidth = 400;
    for (auto&& [_boardConfig, allWpc] : GameCache::get().get_wpc()) {
        for (auto&& wpc : allWpc) {
            wpcViews.push_back(std::make_unique<RectangularBoardView>(
                *wpc,
                SizeInPixels {wpcWidth,
                              size_t(wpcWidth /
                                     rectangularBoardViewWidthToHeightRatio)}));
            save_drawing_area_to_png(wpcViews.back());
        }
    }

    window.show_all();
    return app->run(window);
}
