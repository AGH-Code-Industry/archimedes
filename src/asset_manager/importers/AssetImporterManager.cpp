#include <asset_manager/AssetException.h>
#include <asset_manager/importers/AssetImporterManager.h>

namespace arch::assetManager {

void AssetImporterManager::RegisterImporter(std::unique_ptr<IAssetImporter> importer) noexcept {
	if (importer) {
		arch::Logger::info("Importer registered");
		_importers.emplace_back(std::move(importer));
	} else {
		arch::Logger::error("Importer registration impossible!");
	}
}

void AssetImporterManager::ImportAsset(const std::filesystem::path& sourceFile, arch::assetManager::AssetType assetType)
	const noexcept {
	std::vector<std::string> fileExtensions{};

	switch (assetType) {
		case arch::assetManager::AssetType::MESH:	fileExtensions = { ".obj" }; break;
		case arch::assetManager::AssetType::SHADER: fileExtensions = { ".glsl", ".hlsl" }; break;
		case arch::assetManager::AssetType::FONT:	fileExtensions = { ".ttf", ".otf" }; break;
		default:									arch::Logger::error("Asset type handling not implemented."); return;
	}

	bool assetFound{ false };
	std::filesystem::path finalPath;

	for (std::string extension : fileExtensions) {
		if (std::filesystem::exists(_sourcePathRoot.string() + "/" + sourceFile.string() + extension)) {
			arch::Logger::trace("Asset found('{}').", _sourcePathRoot.string() + "/" + sourceFile.string() + extension);
			finalPath = _sourcePathRoot.string() + "/" + sourceFile.string() + extension;
			assetFound = true;
			break;
		}
	}

	if (!assetFound) {
		arch::Logger::error("Asset not found ('{}').", _sourcePathRoot.string() + "/" + sourceFile.string() + ".*");
		return;
	}


	bool success{ false };

	for (const auto& importer : _importers) {
		if (importer->SupportsFile(finalPath)) {
			if (sourceFile.extension() == ".glsl" || sourceFile.extension() == ".hlsl") {
				importer->SetImportSettings(_shaderImportSettings);
				importer->Import(sourceFile.parent_path(), _processedPathRoot, finalPath);
				arch::Logger::trace("Adequate importer for asset found ('{}').", sourceFile.string());
				success = true;
				break;
			}
			importer->Import(sourceFile.parent_path(), _processedPathRoot, finalPath);
			success = true;
			break;
		}
	}

	if (success) {
		arch::Logger::info("Asset imported ('{}').", finalPath.string());
	} else {
		arch::Logger::warn("Asset couldn't be imported ('{}').", finalPath.string());
	}
}

const std::filesystem::path& AssetImporterManager::GetProcessedPath() const noexcept {
	return _processedPathRoot;
}

const std::filesystem::path& AssetImporterManager::GetSourcePath() const noexcept {
	return _sourcePathRoot;
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

