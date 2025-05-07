#ifndef TOOL_CARD_PATHS_BY_TYPE_H
#define TOOL_CARD_PATHS_BY_TYPE_H

#include "ToolCardType.h"
#include "ViewConstants.h"

#include <unordered_map>
#include <vector>

const std::unordered_map<tc_etype, const std::string> tcImagePaths{
    {tc_etype::CHANGE_DIE_VALUE, v_constants::tcPictureDir + "/GrozingPlier.png"},
    {tc_etype::MOVE_TWICE, v_constants::tcPictureDir + "/RunningPliers.png"},
    {tc_etype::DIE_WITHOUT_NEIGHBORS, v_constants::tcPictureDir + "/Cork-backedStraightedge.png"},
    {tc_etype::REROLL_ALL_DICE, v_constants::tcPictureDir + "/GlazingHammer.png"},
    {tc_etype::RELOCATE_TWO_SAME_COLORED_DICE, v_constants::tcPictureDir + "/TapWheel.png"},
    {tc_etype::REPLACE_FROM_ROUND_TRACK, v_constants::tcPictureDir + "/LensCutter.png"},
    {tc_etype::RANDOM_BAG_DIE, v_constants::tcPictureDir + "/FluxRemover.png"},
    {tc_etype::REROLL_DIE, v_constants::tcPictureDir + "/FluxBrush.png"},
    {tc_etype::REVERSE_DIE_VALUE, v_constants::tcPictureDir + "/GrindingStone.png"},
    {tc_etype::MOVE_IGNORE_VAL, v_constants::tcPictureDir + "/CopperFoilFurnisher.png"},
    {tc_etype::MOVE_IGNORE_COLOR, v_constants::tcPictureDir + "/EglomiseBrush.png"},
    {tc_etype::RELOCATE_TWO_DICE, v_constants::tcPictureDir + "/Lathekin.png"}};

#endif // TOOL_CARD_PATHS_BY_TYPE_H