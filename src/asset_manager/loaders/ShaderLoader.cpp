#include <asset_manager/loaders/ShaderLoader.h>

namespace arch::assetManager {

ShaderLoader::ShaderLoader() {
	arch::Logger::info("Shader Loader created");
}

std::shared_ptr<arch::assetManager::assets::Shader> ShaderLoader::LoadFromFile(const std::filesystem::path& path
) const {
	std::ifstream inStream{ path, std::ios::ate | std::ios::binary };

	if (!std::filesystem::exists(path)) {
		arch::Logger::error("'{}' not found", path.string());
		//throw AssetException("Processed asset not found.");
	}

	if (!inStream) {
		arch::Logger::error("Cannot open '{}'", path.string());
		//throw AssetException("Cannot open processed asset.");
	}

	const std::streamsize size{ inStream.tellg() };

	std::vector<uint32_t> spirvCode(static_cast<size_t>(size / 4));
	inStream.seekg(0, std::ios::beg);
	inStream.read(reinterpret_cast<char*>(spirvCode.data()), size);

	if (!inStream) {
		arch::Logger::error("Error during file reading ('{}').", path.string());
		// throw AssetException("Cannot open processed asset.");
	}

	arch::Logger::trace("Asset passed verification ('{}').", path.string());

	return std::make_shared<arch::assetManager::assets::Shader>(arch::assetManager::assets::Shader(spirvCode));
}

}
