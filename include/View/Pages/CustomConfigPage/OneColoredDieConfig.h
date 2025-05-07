#ifndef ONE_COLORED_DIE_CONFIG_H
#define ONE_COLORED_DIE_CONFIG_H

#include "LabeledWidget.h"
#include "NumberEntry.h"
#include "StringAttributes.h"

class OneColoredDieConfig : public LabeledWidget<NumberEntry> {
  public:
	OneColoredDieConfig(color_t _dieColor, size_t defaultValue)
	    : LabeledWidget<NumberEntry>(color_to_string.at(_dieColor) + " dice count",
	                                 std::to_string(defaultValue)),
	      dieColor(_dieColor)
	{
	}

	const color_t dieColor;
};

#endif // ONE_COLORED_DIE_CONFIG_H