#include <AssetImporterManager.h>
#include <AssetException.h>

namespace arch::assetManager {

void AssetImporterManager::RegisterImporter(std::unique_ptr<IAssetImporter> importer) {
	if (importer) {
		arch::Logger::info(importer->GetType + " registered.")
		_importers.emplace_back(std::move(importer));
	} else {
		throw AssetException("Importer cannot be registered (importer is a nullptr).");
	}
}

void AssetImporterManager::ImportAsset(const std::filesytem::path& sourceFile) const {
	if (!std::filesystem::exists(sourceFile)) {
		throw AssetException(sourceFile.string() + " cannot be found");
	}

	bool success{ false };
	
	for (const auto& importer : _importers) {
		if (importer.SupportsFile(sourceFile)) {
			importer->Import(sourcefile, processedPath);
			success = true;
			break;
		}
	}

	if (success) {
		arch::Logger::info(sourceFile.string() + " loaded succesfully");
	} else {
		arch::Logger::warn(sourceFile.string() + " couldn't be loaded");
	}
}

void AssetImporterManager::SetProcessedPath(const std::filesystem::path& path) {
	_processedPath = path;
}

const std::filesystem::path& AssetImporterManager::GetProcessedPath() const {
	return _processedPath;
}

} // namespace arch::assetManager

