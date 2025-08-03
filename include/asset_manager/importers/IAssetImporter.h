#pragma once

#include <filesystem>
#include <string>
#include <Logger.h>
#include "AssetImporterManager.h"

namespace arch::assetManager {

struct ShaderImportSettings;

class IAssetImporter {
public:
	virtual bool SupportsFile(const std::filesystem::path& path) const noexcept = 0;
	virtual void Import(const std::filesystem::path& sourcePath, const std::filesystem::path& processedPath, const std::filesystem::path& filePath) const noexcept = 0;

	virtual void SetImportSettings(const ShaderImportSettings& importSettings) noexcept {
		arch::Logger::error("This importer shouldn't use shader import settings");
	}

	virtual ~IAssetImporter() = default;
};

} // namespace arch::assetManager
