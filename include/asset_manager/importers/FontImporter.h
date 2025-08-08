#pragma once

#include <asset_manager/AssetEnums.h>

#include "IAssetImporter.h"

// todo		Methods implementation:
// todo			* Path mirroring
// todo			* Read ttf font file (FreeType)
// todo			* Generate texture atlas (msdf-atlas-gen)
// todo			* Serialize .png & .json to minimalistic binary format

namespace arch::assetManager {

class FontImporter: public IAssetImporter {
public:
	bool SupportsFile(const std::filesystem::path& path) const noexcept override;
	void Import(
		const std::filesystem::path& sourcePath,
		const std::filesystem::path& processedPath,
		const std::filesystem::path& filePath
	) const noexcept override;
};

} // namespace arch::assetManager

