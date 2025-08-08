#pragma once

#include <filesystem>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <queue>
#include <optional>

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
	void RegisterLoader(std::unique_ptr<IAssetLoader<T>> loader) noexcept;

	template<class T>
	AssetHandle<T> LoadAsync(const std::filesystem::path& path) noexcept;

	template<class T>
	std::optional<std::shared_ptr<T>> LoadSync(const std::filesystem::path& path) noexcept;

	void TickLoader(std::size_t maxJobs = 4 ) noexcept;

	template<typename TAsset, typename TLoader, typename... TArgs>
	void Register(TArgs&&... args) {
		RegisterLoader<TAsset>(std::make_unique<TLoader>(std::forward<TArgs>(args)...));
	}

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
