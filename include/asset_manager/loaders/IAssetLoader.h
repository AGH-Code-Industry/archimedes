#pragma once

#include <filesystem>
#include <memory>

namespace arch::assetManager {

template <class T>
class IAssetLoader {
public:
	virtual std::shared_ptr<T> LoadFromFile(const std::filesystem::path& path) const = 0;
	virtual ~IAssetLoader() = default;
};

}
