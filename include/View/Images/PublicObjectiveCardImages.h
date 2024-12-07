#ifndef PUBLIC_OBJECTIVE_CARD_IMAGES_H
#define PUBLIC_OBJECTIVE_CARD_IMAGES_H

#include <memory>
#include <vector>

#include "Image.h"
#include "ImageConstants.h"
#include "PublicObjectiveCardPathsByType.h"
#include "PublicObjectiveCardType.h"
#include "PublicObjectiveCardViewConstants.h"

/**
 * @brief Cache for Public objective card images to be loaded only once
 *
 */
struct PublicObjectiveCardImages
{
    static Image& get_concrete(puoc_etype etype)
    {
        if (!instance) {
            load();
        }

        return *instance->images.at(size_t(etype));
    }

    static auto& get_all()
    {
        if (!instance) {
            load();
        }

        return instance->images;
    }

    static void load()
    {
        instance = std::unique_ptr<PublicObjectiveCardImages>(
            new PublicObjectiveCardImages());
    }

  private:
    static std::unique_ptr<PublicObjectiveCardImages> instance;

    std::vector<std::unique_ptr<Image>> images;

    PublicObjectiveCardImages()
    {
        for (size_t i = 0; i < size_t(puoc_etype::COUNT); ++i) {
            images.push_back(std::make_unique<Image>(
                puocImagePaths.at(puoc_etype(i)),
                SizeInPixels {
                    defaulCardtWidth,
                    size_t(defaulCardtWidth / puocWidthToHeightRatio)}));
        }
    }

    PublicObjectiveCardImages(const PublicObjectiveCardImages&) = delete;
    PublicObjectiveCardImages& operator=(const PublicObjectiveCardImages&) =
        delete;
    PublicObjectiveCardImages(PublicObjectiveCardImages&&) = delete;
};

#endif // PUBLIC_OBJECTIVE_CARD_IMAGES_H