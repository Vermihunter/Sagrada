#ifndef SIZE_IN_PIXELS_H
#define SIZE_IN_PIXELS_H

#include <stddef.h>

struct SizeInPixels
{
    using pixel_size_t = size_t;

    pixel_size_t width;
    pixel_size_t height;
};

#endif // SIZE_IN_PIXELS_H