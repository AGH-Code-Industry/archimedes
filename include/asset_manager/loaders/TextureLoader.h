#pragma once

#include <fstream>
#include <string>
#include <memory>

#include <Logger.h>
#include <asset_manager/assets/Texture.h>

#include "IAssetLoader.h"

namespace arch::assetManager {

class TextureLoader: public IAssetLoader<arch::assetManager::assets::Texture> {
public:
	TextureLoader();
	std::shared_ptr<arch::assetManager::assets::Texture> LoadFromFile(const std::filesystem::path& path) const override;
};

} // namespace arch::assetManager
