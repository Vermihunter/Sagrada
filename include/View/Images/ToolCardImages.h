#ifndef TOOL_CARD_IMAGES_H
#define TOOL_CARD_IMAGES_H

#include "Image.h"
#include "ImageConstants.h"
#include "ToolCardPathsByType.h"
#include "ToolCardType.h"
#include "ToolCardViewConstants.h"

#include <memory>
#include <vector>

/**
 * @brief Cache for Tool card images to be loaded only once
 */
struct ToolCardImages {
	static Image& get(tc_etype etype)
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

	static void load() { instance = std::unique_ptr<ToolCardImages>(new ToolCardImages()); }

  private:
	static std::unique_ptr<ToolCardImages> instance;

	std::vector<std::unique_ptr<Image>> images;

	ToolCardImages()
	{
		for (size_t i = 0; i < size_t(tc_etype::COUNT); ++i) {
			images.push_back(std::make_unique<Image>(
			    tcImagePaths.at(tc_etype(i)),
			    SizeInPixels{defaulCardtWidth, size_t(defaulCardtWidth / tcWidthToHeightRatio)}));
		}
	}

	ToolCardImages(const ToolCardImages&)            = delete;
	ToolCardImages& operator=(const ToolCardImages&) = delete;
	ToolCardImages(ToolCardImages&&)                 = delete;
};

#endif // TOOL_CARD_IMAGES_H
