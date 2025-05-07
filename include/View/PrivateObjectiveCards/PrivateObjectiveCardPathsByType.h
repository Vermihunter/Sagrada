#ifndef PRIVATE_OBJECTIVE_CARD_PATHS_BY_TYPE_h
#define PRIVATE_OBJECTIVE_CARD_PATHS_BY_TYPE_h

#include "Attributes.h"
#include "ViewConstants.h"

#include <unordered_map>

const std::unordered_map<color_t, const std::string> procImagePaths{
    {color_t::BLUE, v_constants::procPictureDir + "/CardBlue.png"},
    {color_t::YELLOW, v_constants::procPictureDir + "/CardYellow.png"},
    {color_t::RED, v_constants::procPictureDir + "/CardRed.png"},
    {color_t::PURPLE, v_constants::procPictureDir + "/CardPurple.png"},
    {color_t::GREEN, v_constants::procPictureDir + "/CardGreen.png"}};

#endif // PRIVATE_OBJECTIVE_CARD_PATHS_BY_TYPE_h