#pragma once

#include <filesystem>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <queue>

#include "AssetHandle.h"
#include "loaders/IAssetLoader.h"
#include "loaders/MeshLoader.h"
#include "loaders/ShaderLoader.h"
#include "AssetID.h"


#include <Logger.h>
#include <meta/Rtti.h>

namespace arch::assetManager {

class AssetManager {
public:

	template<class T>
	void RegisterLoader(std::unique_ptr<IAssetLoader<T>> loader);

	template<class T>
	AssetHandle<T> LoadAsync(const std::filesystem::path& path);

	template<class T>
	std::shared_ptr<T> LoadSync(const std::filesystem::path& path);

	void TickLoader(std::size_t maxJobs = 4 );

private:

	struct LoadRequest {
		std::filesystem::path path;
		arch::meta::rtti::TypeDescriptorWrapper type;
		std::shared_ptr<AssetEntryBase> entry;
	};

	std::unordered_map<arch::meta::rtti::TypeDescriptorWrapper, std::unique_ptr<IAssetLoaderBase>> _loaders;
	std::unordered_map<AssetID, std::shared_ptr<AssetEntryBase>> _cache;
	std::queue<LoadRequest> _queue;
};

} // namespace arch::assetManager
