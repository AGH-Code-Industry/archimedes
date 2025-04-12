#pragma once

#import <filesystem>
#include <vector>
#include <memory>

#include "IAssetImporter.h"

namespace arch::assetManager {

class AssetImporterManager {
public:
	AssetImporterManager() = default;
	
	explicit AssetImporterManager(std::filesystem::path processedPath): _processedPath(std::move(processedPath)) {}

	void RegisterImporter(std::unique_ptr<IAssetImporter> importer);
	void ImportAsset(const std::filesytem::path& sourceFile) const;

	void SetProcessedPath(const std::filesystem::path& path);
	const std::filesystem::path& GetProcessedPath() const;


private: 
	std::filesystem::path _processedPath;
	std::vector<std::unique_ptr<IAssetImporter> _importers;
};

}
