#pragma once

#include <filesystem>
#include <fstream>
#include <shaderc/shaderc.hpp>
#include <string>

#include "IAssetImporter.h"
#include <Logger.h>
#include <asset_manager/AssetEnums.h>
#include "AssetImporterManager.h"

namespace arch::assetManager {

class ShaderImporter: public IAssetImporter {
public:
	bool SupportsFile(const std::filesystem::path& path) const override;
	void Import(const std::filesystem::path& sourcePath, const std::filesystem::path& processedPath) const override;
	void SetImportSettings(const ShaderImportSettings& importSettings) override;

private:
	ShaderOptimizationMode _optMode;
	ShaderType _shaderType;
	ShaderSourceLanguage _sourceLanguage;
};

} // namespace arch::assetManager

