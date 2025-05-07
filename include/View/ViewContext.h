#ifndef VIEW_CONTEXT_H
#define VIEW_CONTEXT_H

#include "Point.h"
#include "SizeInPixels.h"

class SagradaWindow;

struct ViewContext {
	ViewContext(const SizeInPixels& sizeInPixels, const Point& _upperLeft,
	            SagradaWindow& _mainWindow);

	~ViewContext();

	ViewContext with_reduced_width(double factor) { return with_reduced_size(factor, 1.0); }

	ViewContext with_reduced_height(double factor) { return with_reduced_size(1.0, factor); }

	ViewContext with_reduced_size(double widthFactor, double heightFactor)
	{
		return ViewContext{
		    SizeInPixels{size_t(size.width * widthFactor), size_t(size.height * heightFactor)},
		    upperLeft, mainWindow};
	}

	const SizeInPixels size;
	const Point        upperLeft;
	SagradaWindow&     mainWindow;
};

#endif // VIEW_CONTEXT_H