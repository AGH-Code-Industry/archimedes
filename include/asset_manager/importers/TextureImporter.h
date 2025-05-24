#pragma once

#include "IAssetImporter.h"

#include <ktx.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize.h>



#include <Logger.h>

namespace arch::assetManager {

class TextureImporter: public IAssetImporter {
public:
	bool SupportsFile(const std::filesystem::path& path) const override;
	void Import(const std::filesystem::path& sourcePath, const std::filesystem::path& processedPath) const override;
};

} // namespace arch::assetManager

