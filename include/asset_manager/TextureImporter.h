#pragma once

#include "IAssetImporter.h"

namespace arch::assetManager {

class TextureImporter: public IAssetImporter {
public:
	bool SupportsFile(const std::filesystem::path& path) const override;
	void Import(const std::filesystem::path& sourcePath, const std::filesystem::path& processedPath) const override;
};

} // namespace arch::assetManager

