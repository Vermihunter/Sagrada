#include "Image.h"

Image::Image(const std::string& imagePath)
{
	auto pixbuf = Gdk::Pixbuf::create_from_file(imagePath);

	set(pixbuf);
}

Image::Image(const std::string& imagePath, SizeInPixels scaleSize)
{
	auto pixbuf = Gdk::Pixbuf::create_from_file(imagePath);
	auto scaledPixbuf =
	    pixbuf->scale_simple(scaleSize.width, scaleSize.height, Gdk::InterpType::INTERP_BILINEAR);

	set(scaledPixbuf);
	show();
}

Image::~Image()
{
	// dtor
}
