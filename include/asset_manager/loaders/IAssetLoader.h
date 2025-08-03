#pragma once

#include <filesystem>
#include <memory>
#include <optional>
#include <typeindex>

namespace arch::assetManager {

class IAssetLoaderBase {
public:
	virtual std::type_index AssetType() const noexcept = 0;
	virtual ~IAssetLoaderBase() = default;
};

template<class T>
class IAssetLoader: public IAssetLoaderBase {
public:
	virtual std::optional<std::shared_ptr<T>> LoadFromFile(const std::filesystem::path& path) const = 0;

	std::type_index AssetType() const noexcept override { return typeid(T); }

	virtual ~IAssetLoader() = default;
};

} // namespace arch::assetManager
