#ifndef ATTRIBUTES_JSON_H
#define ATTRIBUTES_JSON_H

#include "Attributes.h"

#include <nlohmann/json.hpp>

#define JSON_HAS_CPP_17 1
using json = nlohmann::json_abi_v3_11_2::json;

NLOHMANN_JSON_SERIALIZE_ENUM(color_t, {{color_t::COUNT, nullptr},
                                       {color_t::BLUE, "Blue"},
                                       {color_t::GREEN, "Green"},
                                       {color_t::YELLOW, "Yellow"},
                                       {color_t::RED, "Red"},
                                       {color_t::PURPLE, "Purple"}})

NLOHMANN_JSON_SERIALIZE_ENUM(value_t, {{value_t::COUNT, nullptr},
                                       {value_t::ONE, 1},
                                       {value_t::TWO, 2},
                                       {value_t::THREE, 3},
                                       {value_t::FOUR, 4},
                                       {value_t::FIVE, 5},
                                       {value_t::SIX, 6}})

#endif