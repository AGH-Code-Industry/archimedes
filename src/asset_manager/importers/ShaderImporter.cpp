#include <asset_manager/importers/ShaderImporter.h>

namespace arch::assetManager {
bool ShaderImporter::SupportsFile(const std::filesystem::path& path) const noexcept {
	std::string extension{ path.extension().string() };
	if (extension == ".glsl" || extension == ".hlsl") {
		return true;
	} else {
		return false;
	}
	return true;
}

void ShaderImporter::Import(
	const std::filesystem::path& sourcePath,
	const std::filesystem::path& processedPath,
	const std::filesystem::path& filePath
) const noexcept {
	std::ifstream inStream(filePath, std::ios::binary);

	if (!inStream) {
		arch::Logger::error("File not found - '{}'", filePath.string());
		return;
	} else {
		arch::Logger::trace("File succesfully located - '{}'", filePath.string());
	}

	std::string rawShaderCode{ std::istreambuf_iterator<char>(inStream), std::istreambuf_iterator<char>() };

	if (rawShaderCode.empty()) {
		arch::Logger::warn("Shader file empty ('{}'). Check path and shader file content.", sourcePath.string());
	}

	shaderc::Compiler compiler{};
	shaderc::CompileOptions compileOpts{};

	compileOpts.SetSourceLanguage(static_cast<shaderc_source_language>(_sourceLanguage));


	compileOpts.SetOptimizationLevel(static_cast<shaderc_optimization_level>(_optMode));
	compileOpts.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);

	auto result{ compiler.CompileGlslToSpv(
		rawShaderCode,
		static_cast<shaderc_shader_kind>(_shaderType),
		sourcePath.filename().string().c_str(),
		compileOpts
	) };

	if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
		arch::Logger::error("Failed to compile shader ('{}'): {}", filePath.string(), result.GetErrorMessage());
		return;
	} else {
		arch::Logger::trace("Shader compiled succesfully ('{}')", filePath.string());
	}

	std::vector<uint32_t> spirv{ result.begin(), result.end() };

	if (!std::filesystem::exists(processedPath)) {
		std::filesystem::create_directories(processedPath);
	}

	std::string assetPath = processedPath.string() + "/" + sourcePath.string();

	std::filesystem::create_directories(assetPath);
	std::ofstream outStream(assetPath + "/" + filePath.stem().string() + ".spv", std::ios::binary);

	if (!outStream) {
		arch::Logger::error("Mesh file wasn't created");
		return;
	}

	outStream.write(reinterpret_cast<const char*>(spirv.data()), spirv.size() * sizeof(uint32_t));
	arch::Logger::trace("Shader file ('{}') succesfully created!", assetPath + "/" + filePath.stem().string() + ".spv");
}

void ShaderImporter::SetImportSettings(const ShaderImportSettings& importSettings) noexcept {
	_optMode = importSettings._optMode;
	_shaderType = importSettings._shaderType;
	_sourceLanguage = importSettings._sourceLanguage;
}

} // namespace arch::assetManager
