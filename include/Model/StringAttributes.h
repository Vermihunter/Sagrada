#ifndef STRING_ATTRIBUTES_H
#define STRING_ATTRIBUTES_H

#include <string>
#include <unordered_map>

#include "Attributes.h"

const std::unordered_map<color_t, std::string> color_to_string {
    {color_t::BLUE, "Blue"},
    {color_t::RED, "Red"},
    {color_t::GREEN, "Green"},
    {color_t::YELLOW, "Yellow"},
    {color_t::PURPLE, "Purple"}};

const std::unordered_map<value_t, std::string> value_to_string {
    {value_t::ONE, "One"},   {value_t::TWO, "Two"},   {value_t::THREE, "Three"},
    {value_t::FOUR, "Four"}, {value_t::FIVE, "Five"}, {value_t::SIX, "Six"}};

#endif // STRING_ATTRIBUTES_H