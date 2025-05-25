#pragma once

#include "IAssetImporter.h"
#include <ktx.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <vector>

#include <Logger.h>
#include <stb_image_resize.h>

namespace arch::assetManager {

enum class MipMapGenerationMethod : uint8_t {
	DEFAULT,
	BOX_FILTER,
	TRIANGLE_FILTER,
	MITCHELL_NETRAVAL_FILTER,
	LANCZOS3_FILTER,
};

class TextureImporter: public IAssetImporter {
public:
	bool SupportsFile(const std::filesystem::path& path) const override;
	void Import(const std::filesystem::path& sourcePath, const std::filesystem::path& processedPath) const override;
	void SetMipMapGenerationProperties(
		MipMapGenerationMethod genMethod = MipMapGenerationMethod::DEFAULT,
		uint32_t mipMapLevels = 0,
		uint32_t minAllowedDimension = 1
	);

private:

	struct TextureData {
		std::vector<uint8_t> pixels;
		int width;
		int height;
		int channels;
	};

private:

	mutable MipMapGenerationMethod _genMethod{ MipMapGenerationMethod::DEFAULT };
	mutable uint32_t _mipMapLevels = 0;
	mutable uint32_t _minAllowedDimension = 1;
};

} // namespace arch::assetManager

