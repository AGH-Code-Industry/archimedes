#include <asset_manager/importers/AssetImporterManager.h>
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
			if (sourceFile.extension() == ".glsl" || sourceFile.extension() == ".hlsl") {
				importer->SetImportSettings(_shaderImportSettings);
				importer->Import(sourceFile, _processedPath);
				success = true;
				break;
			}
			importer->Import(sourceFile, _processedPath);
			success = true;
			break;
		}
	}

	if (success) {
		arch::Logger::info("Asset imported ('{}').", sourceFile.string());
	} else {
		arch::Logger::warn("Asset couldn't be loaded ('{}').", sourceFile.string());
	}
}

void AssetImporterManager::SetProcessedPath(const std::filesystem::path& path) {
	_processedPath = path;
}

const std::filesystem::path& AssetImporterManager::GetProcessedPath() const {
	return _processedPath;
}

void AssetImporterManager::SetShaderOptimizationMode(const ShaderOptimizationMode& mode) noexcept {
	_shaderImportSettings._optMode = mode;
}

ShaderOptimizationMode AssetImporterManager::GetShaderOptimizationMode() const noexcept {
	std::string mode{};
	switch (_shaderImportSettings._optMode) {
		case ShaderOptimizationMode::NONE:		  mode = "NONE"; break;
		case ShaderOptimizationMode::PERFORMANCE: mode = "PERFORMANCE"; break;
		case ShaderOptimizationMode::SIZE:		  mode = "SIZE"; break;
	}
	arch::Logger::info("Shader optimization mode: {}", mode);
	return _shaderImportSettings._optMode;
}

void AssetImporterManager::SetShaderType(const ShaderType& type) noexcept {
	_shaderImportSettings._shaderType = type;
}

ShaderType AssetImporterManager::GetShaderType() const noexcept {
	std::string type{};
	switch (_shaderImportSettings._shaderType) {
		case ShaderType::VERTEX:   type = "VERTEX"; break;
		case ShaderType::FRAGMENT: type = "FRAGMENT"; break;
		case ShaderType::COMPUTE:  type = "COMPUTE"; break;
	}
	arch::Logger::info("Shader type: {}", type);
	return _shaderImportSettings._shaderType;
}

void AssetImporterManager::SetShaderSourceLanguage(const ShaderSourceLanguage& lang) noexcept {
	_shaderImportSettings._sourceLanguage = lang;
}

ShaderSourceLanguage AssetImporterManager::GetShaderSourceLanguage() const noexcept {
	std::string lang{};
	switch (_shaderImportSettings._sourceLanguage) {
		case ShaderSourceLanguage::GLSL: lang = "GLSL"; break;
		case ShaderSourceLanguage::HLSL: lang = "HLSL"; break;
	}
	arch::Logger::info("Shader source language: {}", lang);
	return _shaderImportSettings._sourceLanguage;
}

void AssetImporterManager::SetShaderImportSettings(
	const ShaderOptimizationMode& mode,
	const ShaderType& type,
	const ShaderSourceLanguage& lang
) noexcept {
	_shaderImportSettings._optMode = mode;
	_shaderImportSettings._shaderType = type;
	_shaderImportSettings._sourceLanguage = lang;
}

ShaderImportSettings AssetImporterManager::GetShaderImportSettings() const noexcept {
	ShaderImportSettings settings(
		_shaderImportSettings._optMode,
		_shaderImportSettings._shaderType,
		_shaderImportSettings._sourceLanguage
	);

	std::string mode{};
	switch (_shaderImportSettings._optMode) {
		case ShaderOptimizationMode::NONE:		  mode = "NONE"; break;
		case ShaderOptimizationMode::PERFORMANCE: mode = "PERFORMANCE"; break;
		case ShaderOptimizationMode::SIZE:		  mode = "SIZE"; break;
	}

	std::string type{};
	switch (_shaderImportSettings._shaderType) {
		case ShaderType::VERTEX:   type = "VERTEX"; break;
		case ShaderType::FRAGMENT: type = "FRAGMENT"; break;
		case ShaderType::COMPUTE:  type = "COMPUTE"; break;
	}

	std::string lang{};
	switch (_shaderImportSettings._sourceLanguage) {
		case ShaderSourceLanguage::GLSL: lang = "GLSL"; break;
		case ShaderSourceLanguage::HLSL: lang = "HLSL"; break;
	}

	arch::Logger::info("Shader optimization mode: {}. Shader type: {}. Shader source language: {}.", mode, type, lang);

	return settings;
}

} // namespace arch::assetManager

