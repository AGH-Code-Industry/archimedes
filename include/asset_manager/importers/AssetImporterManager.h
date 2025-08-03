#pragma once

#include <filesystem>
#include <vector>
#include <memory>

#include <Logger.h>

#include "IAssetImporter.h"
#include <asset_manager/AssetEnums.h>

namespace arch::assetManager {

class IAssetImporter;

struct ShaderImportSettings {
	ShaderImportSettings(const ShaderOptimizationMode& mode, const ShaderType& type, const ShaderSourceLanguage& lang):
		_optMode(mode),
		_shaderType(type),
		_sourceLanguage(lang) {}

	ShaderImportSettings():
		_optMode(ShaderOptimizationMode::NOT_SET),
		_shaderType(ShaderType::NOT_SET),
		_sourceLanguage(ShaderSourceLanguage::NOT_SET) {}

	ShaderOptimizationMode _optMode;
	ShaderType _shaderType;
	ShaderSourceLanguage _sourceLanguage;
};

class AssetImporterManager {
public:
	AssetImporterManager() = default;
	
	explicit AssetImporterManager(std::filesystem::path sourcePath, std::filesystem::path processedPath):
		_sourcePathRoot(std::move(sourcePath)),
		_processedPathRoot(std::move(processedPath)) {}

	void RegisterImporter(std::unique_ptr<IAssetImporter> importer) noexcept;
	void ImportAsset(const std::filesystem::path& sourceFile, arch::assetManager::AssetType assetType) const noexcept;

	const std::filesystem::path& GetProcessedPath() const noexcept;

	const std::filesystem::path& GetSourcePath() const noexcept;

	void SetShaderOptimizationMode(const ShaderOptimizationMode& mode) noexcept;
	ShaderOptimizationMode GetShaderOptimizationMode() const noexcept;

	void SetShaderType(const ShaderType& type) noexcept;
	ShaderType GetShaderType() const noexcept;

	void SetShaderSourceLanguage(const ShaderSourceLanguage& lang) noexcept;
	ShaderSourceLanguage GetShaderSourceLanguage() const noexcept;

	void SetShaderImportSettings(
		const ShaderOptimizationMode& mode,
		const ShaderType& type,
		const ShaderSourceLanguage& lang
	) noexcept;

	ShaderImportSettings GetShaderImportSettings() const noexcept;

private: 
	std::filesystem::path _processedPathRoot;
	std::filesystem::path _sourcePathRoot;
	std::vector<std::unique_ptr<IAssetImporter>> _importers;

	ShaderImportSettings _shaderImportSettings;
};

}
