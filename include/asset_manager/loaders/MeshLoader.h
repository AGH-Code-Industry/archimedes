#pragma once

#include <fstream>
#include <string>
#include <array>
#include <cstdint>
#include <memory>

#include <Logger.h>
#include <asset_manager/assets/Mesh.h>

#include "IAssetLoader.h"

namespace arch::assetManager {

class MeshLoader: public IAssetLoader<Mesh> {
public:
	MeshLoader();
	std::shared_ptr<Mesh> LoadFromFile(const std::filesystem::path& path) const override;
};

}
