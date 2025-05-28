#include <asset_manager/importers/ShaderImporter.h>

namespace arch::assetManager {
bool ShaderImporter::SupportsFile(const std::filesystem::path& path) const {
	arch::Logger::info("Shader checking not ready yet.");
}

void ShaderImporter::Import(const std::filesystem::path& sourcePath, const std::filesystem::path& processedPath) const {
	std::ifstream inStream(sourcePath, std::ios::binary);

	if (!inStream) {
		arch::Logger::error("File not found - '{}'", sourcePath.string());
		return;
	} else {
		arch::Logger::trace("File succesfully located - '{}'", sourcePath.string());
	}

	std::string rawShaderCode{ std::istreambuf_iterator<char>(inStream), std::istreambuf_iterator<char>() };

	if (rawShaderCode.empty()) {
		arch::Logger::warn("Shader file empty ('{}'). Check path and shader file content.", sourcePath.string());
	}

	shaderc::Compiler compiler{};
	shaderc::CompileOptions compileOpts{};

	if (sourcePath.extension().string() == ".hlsl") {
		compileOpts.SetSourceLanguage(shaderc_source_language_hlsl);
	} else {
		compileOpts.SetSourceLanguage(shaderc_source_language_glsl);
	}

	compileOpts.SetOptimizationLevel(static_cast<shaderc_optimization_level>(_optMode));
	compileOpts.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);

	auto result{ compiler.CompileGlslToSpv(
		rawShaderCode,
		static_cast<shaderc_shader_kind>(_shaderType),
		sourcePath.filename().string().c_str(),
		compileOpts
	) };

	if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
		arch::Logger::error("Failed to compile shader ('{}'): {}", sourcePath.string(), result.GetErrorMessage());
		return;
	} else {
		arch::Logger::trace("Shader compiled succesfully ('{}')", sourcePath.string());
	}

	std::vector<uint32_t> spirv{ result.begin(), result.end() };

	std::ofstream outStream(processedPath, std::ios::binary);
	
	if (!outStream) {
		arch::Logger::error("SPIR-V shader file wasn't created ('{}')", processedPath.string());
		return;
	}

	outStream.write(reinterpret_cast<const char*>(spirv.data()), spirv.size() * sizeof(uint32_t));
	arch::Logger::info("Shader file ('{}') succesfully created!", processedPath.string());
}

void ShaderImporter::SetOptimizationMode(const ShaderOptimizationMode& mode) noexcept {
	_optMode = mode;
}

ShaderOptimizationMode ShaderImporter::GetOptimizationMode() const noexcept {
	std::string mode{};
	switch (_optMode) {
		case ShaderOptimizationMode::NONE:		  mode = "NONE"; break;
		case ShaderOptimizationMode::PERFORMANCE: mode = "PERFORMANCE"; break;
		case ShaderOptimizationMode::SIZE:		  mode = "SIZE"; break;
	}
	arch::Logger::info("Shader optimization mode: {}", mode);
	return _optMode;
}

void ShaderImporter::SetShaderType(const ShaderType& type) noexcept {
	_shaderType = type;
}

ShaderType ShaderImporter::GetShaderType() const noexcept {
	std::string type{};
	switch (_shaderType) {
		case ShaderType::VERTEX:   type = "VERTEX"; break;
		case ShaderType::FRAGMENT: type = "FRAGMENT"; break;
		case ShaderType::COMPUTE:  type = "COMPUTE"; break;
	}
	arch::Logger::info("Shader type: {}", type);
	return _shaderType;
}

void ShaderImporter::SetSourceLanguage(const ShaderSourceLanguage& lang) noexcept {
	_sourceLanguage = lang;
}

ShaderSourceLanguage ShaderImporter::GetSourceLanguage() const noexcept {
	std::string lang{};
	switch (_sourceLanguage) {
		case ShaderSourceLanguage::GLSL: lang = "GLSL"; break;
		case ShaderSourceLanguage::HLSL: lang = "HLSL"; break;
	}
	arch::Logger::info("Shader source language: {}", lang);
	return _sourceLanguage;
}

void ShaderImporter::SetImportSettings(
	const ShaderOptimizationMode& mode,
	const ShaderType& type,
	const ShaderSourceLanguage& lang
) noexcept {
	_optMode = mode;
	_shaderType = type;
	_sourceLanguage = lang;
}

ShaderImportSettings ShaderImporter::GetImportSettings() const noexcept {
	ShaderImportSettings settings(_optMode, _shaderType, _sourceLanguage);

	std::string mode{};
	switch (_optMode) {
		case ShaderOptimizationMode::NONE:		  mode = "NONE"; break;
		case ShaderOptimizationMode::PERFORMANCE: mode = "PERFORMANCE"; break;
		case ShaderOptimizationMode::SIZE:		  mode = "SIZE"; break;
	}

	std::string type{};
	switch (_shaderType) {
		case ShaderType::VERTEX:   type = "VERTEX"; break;
		case ShaderType::FRAGMENT: type = "FRAGMENT"; break;
		case ShaderType::COMPUTE:  type = "COMPUTE"; break;
	}

	std::string lang{};
	switch (_sourceLanguage) {
		case ShaderSourceLanguage::GLSL: lang = "GLSL"; break;
		case ShaderSourceLanguage::HLSL: lang = "HLSL"; break;
	}

	arch::Logger::info("Shader optimization mode: {}. Shader type: {}. Shader source language: {}.", mode, type, lang);

	return settings;
}


} // namespace arch::assetManager
