#include <asset_manager/AssetManager.h>

namespace arch::assetManager {

template<class T>
void AssetManager::RegisterLoader(std::unique_ptr<IAssetLoader<T>> loader) {
	_loaders.emplace(loader->AssetType(), std::move(loader));
	arch::Logger::info("Loader registered in AssetManager.");
}

template<class T>
AssetHandle<T> AssetManager::LoadAsync(const std::filesystem::path& path) {
	AssetID id(path.string());

	auto& entryBase{ _cache[id] };
	if (!entryBase) {
		entryBase = std::make_shared<AssetEntry<T>>();
		Logger::trace("Asset entry created.");
	}

	auto entry{ std::static_pointer_cast<AssetEntry<T>>(entryBase) };

	if (!entry->loadQueued) {
		entry->loadQueued = true;
		_queue.push({ path, typeid(T), entry });
		Logger::trace("Asset loaded into queue.");
	}

	return AssetHandle<T>{ entryBase };
}

template<class T>
std::shared_ptr<T> AssetManager::LoadSync(const std::filesystem::path& path) {
	auto h{ LoadAsync<T>(path) };

	TickLoader(std::numeric_limits<std::size_t>::max());
	if (!h.IsReady()) {
		throw AssetException("Asset " + path.string() + " failed to load: " + h.ErrorMsg());
	}

	return std::static_pointer_cast<AssetEntry<T>>(h._entry)->ptr;
}

void AssetManager::TickLoader(std::size_t maxJobs) {
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
			auto* loader{ static_cast<IAssetLoader<Mesh>*>(base) };
			auto assetPtr{ loader->LoadFromFile(job.path) };

			auto* entryT = static_cast<AssetEntry<Mesh>*>(job.entry.get());
			entryT->ptr = std::move(assetPtr);
			job.entry->state = AssetState::Ready;
		}
		catch (const std::exception& e) {
			job.entry->state = AssetState::Error;
			job.entry->err = e.what();
			Logger::error("Asset load error: {}", e.what());
		}
	}
}

template void AssetManager::RegisterLoader<Mesh>(std::unique_ptr<IAssetLoader<Mesh>>);
template AssetHandle<Mesh> AssetManager::LoadAsync<Mesh>(const std::filesystem::path&);
template std::shared_ptr<Mesh> AssetManager::LoadSync<Mesh>(const std::filesystem::path&);

}
