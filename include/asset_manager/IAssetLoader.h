#pragma once

#include <filesystem>

namespace arch::assetManager {

class IAssetLoader {
public:
	virtual bool LoadFromFile(std::filesystem::path path) const = 0;
	virtual ~IAssetLoader() = default;
};

}
