#pragma once

#include <fstream>
#include <string>
#include <array>
#include <cstdint>
#include <memory>
#include <vector>

#include <Logger.h>
#include <gfx/Renderer.h>
#include <asset_manager/assets/Mesh.h>
#include <asset_manager/AssetException.h>

#include "IAssetLoader.h"

namespace arch::assetManager {

class MeshLoader: public IAssetLoader<arch::assetManager::assets::Mesh> {
public:
	MeshLoader();
	std::shared_ptr<arch::assetManager::assets::Mesh> LoadFromFile(const std::filesystem::path& path) const override;
};

}
