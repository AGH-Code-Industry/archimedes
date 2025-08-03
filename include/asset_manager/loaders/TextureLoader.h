#pragma once

#include <fstream>
#include <memory>
#include <optional>
#include <string>

#include "IAssetLoader.h"
#include <Logger.h>
#include <asset_manager/assets/Texture.h>

namespace arch::assetManager {

class TextureLoader: public IAssetLoader<arch::assetManager::assets::Texture> {
public:
	TextureLoader();
	std::optional<std::shared_ptr<arch::assetManager::assets::Texture>> LoadFromFile(const std::filesystem::path& path
	) const override;
};

} // namespace arch::assetManager
