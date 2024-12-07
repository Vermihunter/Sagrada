#ifndef VIEW_CONSTANTS_H
#define VIEW_CONSTANTS_H

#include "Constants.h"
#include "Point.h"
#include "SizeInPixels.h"

namespace v_constants
{
/** View paths */
const std::string imageDir = dataDir + "/Images";
const std::string tcPictureDir = imageDir + "/ToolCards";
const std::string tcIconPath = tcPictureDir + "/Icon.png";

const std::string puocPictureDir = imageDir + "/PublicObjectiveCards";
const std::string puocIconPath = puocPictureDir + "/Icon.png";

const std::string procPictureDir = imageDir + "/PrivateObjectiveCards";
const std::string procIconPath = procPictureDir + "/Icon.png";

constexpr SizeInPixels iconSize {.width = 70, .height = 70};

constexpr Point roundTrackUpperLeft {.x = 1180, .y = 10};

constexpr SizeInPixels roundNumberSize {.width = 150, .height = 100};

namespace sagradaLogo
{
const std::string path = imageDir + "/logo.png";

constexpr SizeInPixels size {
    .width = 600,
    .height = SizeInPixels::pixel_size_t(600 / (2.14486638537))};
} // namespace sagradaLogo

/** Application window config*/
namespace window
{
constexpr SizeInPixels size {
    .width = 1280,
    //  .height = SizeInPixels::pixel_size_t(1280 / (16.0 / 9.0))
    .height = 840};
}
/** Sagrada logo config */

/** Window Pattern Card config */
namespace windowPatternCard
{
constexpr SizeInPixels size {.width = 440,
                             .height = SizeInPixels::pixel_size_t(440 / 1.1)};
}

/** Button config */
namespace button
{
constexpr SizeInPixels size {.width = 350, .height = 75};

constexpr size_t spaceBetweenInPixels = 20;
} // namespace button

} // namespace v_constants

#endif // VIEW_CONSTANTS_H