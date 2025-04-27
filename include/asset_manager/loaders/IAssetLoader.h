#pragma once

#include <filesystem>
#include <memory>
#include <typeindex>

namespace arch::assetManager {

class IAssetLoaderBase {
public:
	virtual std::type_index AssetType() const noexcept = 0;
	virtual std::shared_ptr<void> LoadRaw(const std::filesystem::path& path) const = 0;
	virtual ~IAssetLoaderBase() = default;

};

template <class T>
class IAssetLoader : public IAssetLoaderBase {
public:
	virtual std::shared_ptr<T> LoadFromFile(const std::filesystem::path& path) const = 0;

	std::type_index AssetType() const noexcept override { return typeid(T); } 
	std::shared_ptr<void> LoadRaw(const std::filesystem::path& path) const override { return LoadFromFile(path); }

	virtual ~IAssetLoader() = default;
};	

}
