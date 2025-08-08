#pragma once

#include <asset_manager/AssetEnums.h>

#include "IAssetLoader.h"

// todo		Load binary format into Font class and return it.

namespace arch::assetManager {

class Font: public IAssetLoader<arch::assetManager::assets::Font> {
public:
	MeshLoader(std::filesystem::path processedPath);
	std::optional<std::shared_ptr<arch::assetManager::assets::Font>> LoadFromFile(const std::filesystem::path& path
	) const noexcept override;

private:
	std::filesystem::path _processedPath;
};
