#pragma once

#include <filesystem>
#include <fstream>
#include <shaderc/shaderc.hpp>
#include <string>

#include "IAssetImporter.h"
#include <Logger.h>
#include <asset_manager/AssetEnums.h>

namespace arch::assetManager {

struct ShaderImportSettings {
	ShaderImportSettings(const ShaderOptimizationMode& mode, const ShaderType& type, const ShaderSourceLanguage& lang):
		_optMode(mode),
		_shaderType(type),
		_sourceLanguage(lang) {}

	ShaderOptimizationMode _optMode;
	ShaderType _shaderType;
	ShaderSourceLanguage _sourceLanguage;
};

class ShaderImporter: public IAssetImporter {
public:
	bool SupportsFile(const std::filesystem::path& path) const override;
	void Import(const std::filesystem::path& sourcePath, const std::filesystem::path& processedPath) const override;

	void SetOptimizationMode(const ShaderOptimizationMode& mode) noexcept;
	ShaderOptimizationMode GetOptimizationMode() const noexcept;

	void SetShaderType(const ShaderType& type) noexcept;
	ShaderType GetShaderType() const noexcept;

	void SetSourceLanguage(const ShaderSourceLanguage& lang) noexcept;
	ShaderSourceLanguage GetSourceLanguage() const noexcept;

	void SetImportSettings(
		const ShaderOptimizationMode& mode,
		const ShaderType& type,
		const ShaderSourceLanguage& lang
	) noexcept;

	ShaderImportSettings GetImportSettings() const noexcept;

private:
	ShaderOptimizationMode _optMode{ ShaderOptimizationMode::NONE };
	ShaderType _shaderType;
	ShaderSourceLanguage _sourceLanguage{ ShaderSourceLanguage::GLSL };
};

} // namespace arch::assetManager

