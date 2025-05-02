#pragma once

#include <fstream>
#include <string>
#include <array>
#include <cstdint>
#include <memory>

#include <Logger.h>
#include <gfx/Renderer.h>
#include <asset/mesh/Mesh.h>
#include <asset_manager/AssetException.h>

#include "IAssetLoader.h"

namespace arch::assetManager {

class MeshLoader: public IAssetLoader<arch::asset::mesh::Mesh> {
public:
	MeshLoader();
	std::shared_ptr<arch::asset::mesh::Mesh> LoadFromFile(const std::filesystem::path& path) const override;
};

}
