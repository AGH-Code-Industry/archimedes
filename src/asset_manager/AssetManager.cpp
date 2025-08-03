#include <asset_manager/AssetManager.h>

namespace arch::assetManager {

template<class T>
void AssetManager::RegisterLoader(std::unique_ptr<IAssetLoader<T>> loader) noexcept {
	auto typeWrapper{ arch::meta::rtti::typedesc<T>().wrap() };

	if (_loaders.contains(typeWrapper)) {
		Logger::warn("Loder of type '{}' already registered. Overwriting.", typeWrapper.name());
	}

	_loaders.emplace(typeWrapper, std::move(loader));
	Logger::info("Registered loader for asset type: '{}'", typeWrapper.name());
}

template<class T>
AssetHandle<T> AssetManager::LoadAsync(const std::filesystem::path& path) noexcept {
	AssetID id(path.string());

	auto& entryBase{ _cache[id] };
	if (!entryBase) {
		entryBase = std::make_shared<AssetEntry<T>>();
		Logger::trace("Asset ({}) entry created.", path.string());
	}

	auto entry{ std::static_pointer_cast<AssetEntry<T>>(entryBase) };

	if (!entry->loadQueued) {
		entry->loadQueued = true;
		_queue.push({ path, arch::meta::rtti::typedesc<T>().wrap(), entry });
		Logger::trace("Asset ({}) loaded into queue.", path.string());
	}

	return AssetHandle<T>{ entryBase };
}

template<class T>
std::optional<std::shared_ptr<T>> AssetManager::LoadSync(const std::filesystem::path& path) noexcept {
	auto h{ LoadAsync<T>(path) };

	TickLoader(std::numeric_limits<std::size_t>::max());
	if (!h.IsReady()) {
		Logger::error("Asset ({}) failed to load: {}", path.string(), h.ErrorMsg());
		return {};
	}

	return std::static_pointer_cast<AssetEntry<T>>(h._entry)->ptr;
}

void AssetManager::TickLoader(std::size_t maxJobs) noexcept {
	while (maxJobs-- && !_queue.empty()) {
		auto job{ _queue.front() };
		_queue.pop();

		auto itLoader{ _loaders.find(job.type) };
		if (itLoader == _loaders.end()) {
			job.entry->state = AssetState::Error;
			Logger::error("Loader for desired type not registered.");
			job.entry->err = "no loader registered";
			continue;
		}

		try {
			auto* base{ itLoader->second.get() };
			auto* loader{ static_cast<IAssetLoader<arch::assetManager::assets::Mesh>*>(base) };
			auto assetPtr{ loader->LoadFromFile(job.path) };

			auto* entryT = static_cast<AssetEntry<arch::assetManager::assets::Mesh>*>(job.entry.get());
			if (assetPtr) {
				entryT->ptr = std::move(*assetPtr);
			} else {
				entryT->ptr = nullptr;
			}
			job.entry->state = AssetState::Ready;
		} catch (const std::exception& e) {
			job.entry->state = AssetState::Error;
			job.entry->err = e.what();
			Logger::error("Asset load error: {}", e.what());
		}
	}
}

template void AssetManager::RegisterLoader<
	arch::assetManager::assets::Mesh>(std::unique_ptr<IAssetLoader<arch::assetManager::assets::Mesh>>);
template AssetHandle<arch::assetManager::assets::Mesh> AssetManager::LoadAsync<
	arch::assetManager::assets::Mesh>(const std::filesystem::path&);
template std::optional<std::shared_ptr<arch::assetManager::assets::Mesh>> AssetManager::LoadSync<
	arch::assetManager::assets::Mesh>(const std::filesystem::path&);

template void AssetManager::RegisterLoader<
	arch::assetManager::assets::Shader>(std::unique_ptr<IAssetLoader<arch::assetManager::assets::Shader>>);
template AssetHandle<arch::assetManager::assets::Shader> AssetManager::LoadAsync<
	arch::assetManager::assets::Shader>(const std::filesystem::path&);
template std::optional<std::shared_ptr<arch::assetManager::assets::Shader>> AssetManager::LoadSync<
	arch::assetManager::assets::Shader>(const std::filesystem::path&);

} // namespace arch::assetManager
