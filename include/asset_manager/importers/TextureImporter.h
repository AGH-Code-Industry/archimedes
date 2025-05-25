#pragma once

#include "IAssetImporter.h"

#include <ktx.h>
#include <vulkan/vulkan_core.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <vector>
#include <cmath>

#include <Logger.h>

namespace arch::assetManager {

class TextureImporter: public IAssetImporter {
public:
	bool SupportsFile(const std::filesystem::path& path) const override;
	void Import(const std::filesystem::path& sourcePath, const std::filesystem::path& processedPath) const override;
};

} // namespace arch::assetManager

