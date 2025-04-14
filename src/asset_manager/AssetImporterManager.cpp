#include <asset_manager/AssetImporterManager.h>
#include <asset_manager/AssetException.h>

namespace arch::assetManager {

void AssetImporterManager::RegisterImporter(std::unique_ptr<IAssetImporter> importer) {
	if (importer) {
		arch::Logger::info("Importer registered");
		_importers.emplace_back(std::move(importer));
	} else {
		throw AssetException("Importer cannot be registered (importer is a nullptr).");
	}
}

void AssetImporterManager::ImportAsset(const std::filesystem::path& sourceFile) const {
	if (!std::filesystem::exists(sourceFile)) {
		//throw AssetException(sourceFile.string() + " cannot be found");
		arch::Logger::warn("Asset not found.");
	} else {
		arch::Logger::trace("Asset found.");
	}

	bool success{ false };
	
	for (const auto& importer : _importers) {
		if (importer->SupportsFile(sourceFile)) {
			importer->Import(sourceFile, _processedPath);
			success = true;
			break;
		}
	}

	if (success) {
		arch::Logger::info("Asset loaded ");
	} else {
		arch::Logger::warn("Asset couldn't be loaded");
	}
}

void AssetImporterManager::SetProcessedPath(const std::filesystem::path& path) {
	_processedPath = path;
}

const std::filesystem::path& AssetImporterManager::GetProcessedPath() const {
	return _processedPath;
}

} // namespace arch::assetManager

