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
#include <asset_manager/assets/Mesh.h>
#include <gfx/Renderer.h>

namespace arch::assetManager {

class MeshLoader: public IAssetLoader<arch::assetManager::assets::Mesh> {
public:
	MeshLoader(std::filesystem::path processedPath);
	std::optional<std::shared_ptr<arch::assetManager::assets::Mesh>> LoadFromFile(const std::filesystem::path& path
	) const noexcept override;

private:
	std::filesystem::path _processedPath;
};

} // namespace arch::assetManager
