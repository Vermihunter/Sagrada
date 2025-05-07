#ifndef SIMPLE_AI_TYPE_SPECIFIER_VIEW_H
#define SIMPLE_AI_TYPE_SPECIFIER_VIEW_H

#include "AI_TypeSpecifierView.h"

// Used for any AI type that has a config with default constructor -> First and
// Random
template <typename ConfigType> class SimpleAI_TypeSpecifierView : public AI_TypeSpecifierView {
  public:
	virtual ai_player_config_t create_player() override { return std::make_unique<ConfigType>(); }
};

#endif // SIMPLE_AI_TYPE_SPECIFIER_VIEW_H