#include <asset_manager/loaders/ShaderLoader.h>

namespace arch::assetManager {

ShaderLoader::ShaderLoader(std::filesystem::path processedPath): _processedPath(processedPath) {
	arch::Logger::trace("Shader Loader created");
}

std::optional<std::shared_ptr<arch::assetManager::assets::Shader>> ShaderLoader::LoadFromFile(const std::filesystem::path& path
) const noexcept {

	std::filesystem::path finalPath{ _processedPath.string() + "/" + path.string() + ".spv" };

	std::ifstream inStream{ finalPath, std::ios::ate | std::ios::binary };

	if (!std::filesystem::exists(finalPath)) {
		arch::Logger::error("'{}' not found", finalPath.string());
		return {};
	}

	if (!inStream) {
		arch::Logger::error("Cannot open '{}'", finalPath.string());
		return {};
	}

	const std::streamsize size{ inStream.tellg() };

	std::vector<uint32_t> spirvCode(static_cast<size_t>(size / 4));
	inStream.seekg(0, std::ios::beg);
	inStream.read(reinterpret_cast<char*>(spirvCode.data()), size);

	if (!inStream) {
		arch::Logger::error("Error during file reading ('{}').", finalPath.string());
		return {};
	}

	arch::Logger::trace("Asset passed verification ('{}').", finalPath.string());

	return std::make_shared<arch::assetManager::assets::Shader>(arch::assetManager::assets::Shader(spirvCode));
}

} // namespace arch::assetManager
