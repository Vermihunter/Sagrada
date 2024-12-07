#ifndef IMAGE_H
#define IMAGE_H

#include <gtkmm.h>

#include "SizeInPixels.h"

/**
 * @brief Wrapper class around Gtk::Image
 * Defined for easier Image definition using scaling factors
 */
class Image : public Gtk::Image
{
  public:
    /** Constructors */
    Image(const std::string& imagePath);
    Image(const std::string& imagePath, SizeInPixels scaleSize);

    /** Default destructor */
    virtual ~Image();
};

#endif // IMAGE_H
