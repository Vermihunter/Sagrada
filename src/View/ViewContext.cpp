#include "ViewContext.h"

#include "SagradaWindow.h"

ViewContext::ViewContext(const SizeInPixels& sizeInPixels,
                         const Point& _upperLeft, SagradaWindow& _mainWindow)
    : size(sizeInPixels), upperLeft(_upperLeft), mainWindow(_mainWindow)
{
}

ViewContext::~ViewContext() {}