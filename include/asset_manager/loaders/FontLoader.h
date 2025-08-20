#pragma once

#include <array>
#include <cstdint>
#include <fstream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "IAssetLoader.h"
#include <Logger.h>
#include <asset_manager/AssetException.h>
#include <asset_manager/assets/Font.h>

#include "IAssetLoader.h"

// todo		Load binary format into Font class and return it.

namespace arch::assetManager {

class FontLoader: public IAssetLoader<arch::assetManager::assets::Font> {
public:
	FontLoader(std::filesystem::path processedPath);
	std::optional<std::shared_ptr<arch::assetManager::assets::Font>> LoadFromFile(const std::filesystem::path& path
	) const noexcept override;

private:
	std::filesystem::path _processedPath;
};

} // namespace arch::assetManager
