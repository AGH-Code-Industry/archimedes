#include <asset_manager/importers/ShaderImporter.h>

namespace arch::assetManager {
bool ShaderImporter::SupportsFile(const std::filesystem::path& path) const {
	std::string extension{ path.extension().string() };
	if (extension == ".glsl" || extension == ".hlsl") {
		return true;
	} else {
		return false;
	}
	return true;
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
		arch::Logger::error("Failed to compile shader ('{}'): {}", sourcePath.string(), result.GetErrorMessage());
		return;
	} else {
		arch::Logger::trace("Shader compiled succesfully ('{}')", sourcePath.string());
	}

	std::vector<uint32_t> spirv{ result.begin(), result.end() };

	std::string outPath{ processedPath.string() + "/shaders/" + sourcePath.stem().string() + ".spv" };

	std::filesystem::create_directory(processedPath.string() + "/shaders");
	std::ofstream outStream(
		outPath,
		std::ios::binary
	);
	
	if (!outStream) {
		arch::Logger::error("SPIR-V shader file wasn't created ('{}')", outPath);
		return;
	}

	outStream.write(reinterpret_cast<const char*>(spirv.data()), spirv.size() * sizeof(uint32_t));
	arch::Logger::info("Shader file ('{}') succesfully created!", outPath);
}

void ShaderImporter::SetImportSettings(const ShaderImportSettings& importSettings) {
	_optMode = importSettings._optMode;
	_shaderType = importSettings._shaderType;
	_sourceLanguage = importSettings._sourceLanguage;
}

} // namespace arch::assetManager
