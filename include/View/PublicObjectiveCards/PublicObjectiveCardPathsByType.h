#ifndef PUBLIC_OBJECTIVE_CARD_PATHS_BY_TYPE_h
#define PUBLIC_OBJECTIVE_CARD_PATHS_BY_TYPE_h

#include "PublicObjectiveCardType.h"
#include "ViewConstants.h"

#include <unordered_map>

const std::unordered_map<puoc_etype, const std::string> puocImagePaths{
    {puoc_etype::UNIQUE_VALUE_ROW, v_constants::puocPictureDir + "/RowShadeVariety.png"},
    {puoc_etype::UNIQUE_COLOR_ROW, v_constants::puocPictureDir + "/RowColorVariety.png"},
    {puoc_etype::UNIQUE_VALUE_COLUMN, v_constants::puocPictureDir + "/ColumnShadeVariety.png"},
    {puoc_etype::UNIQUE_COLOR_COLUMN, v_constants::puocPictureDir + "/ColumnColorVariety.png"},
    {puoc_etype::ONE_AND_TWO_COLLECTING, v_constants::puocPictureDir + "/LightShades.png"},
    {puoc_etype::THREE_AND_FOUR_COLLECTING, v_constants::puocPictureDir + "/MediumShades.png"},
    {puoc_etype::FIVE_AND_SIX_COLLECTING, v_constants::puocPictureDir + "/DeepShades.png"},
    {puoc_etype::ALL_VALUE_COLLECTING, v_constants::puocPictureDir + "/ShadeVariety.png"},
    {puoc_etype::ALL_COLOR_COLLECTING, v_constants::puocPictureDir + "/ColorVariety.png"},
    {puoc_etype::DIAGONAL_CONNECTING, v_constants::puocPictureDir + "/ColorDiagonals.png"}};

#endif // PUBLIC_OBJECTIVE_CARD_PATHS_BY_TYPE_h