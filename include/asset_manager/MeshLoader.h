#pragma once

#include <fstream>
#include <string>
#include <array>
#include <cstdint>

#include <Logger.h>

#include "IAssetLoader.h"

namespace arch::assetManager {

class MeshLoader : public IAssetLoader {
public:
	MeshLoader();
	bool LoadFromFile(std::filesystem::path path) const override;
};

}
