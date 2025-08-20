#include <asset_manager/loaders/FontLoader.h>

namespace arch::assetManager {

FontLoader::FontLoader(std::filesystem::path processedPath): _processedPath(processedPath) {
	arch::Logger::trace("Mesh Loader created");
}

std::optional<std::shared_ptr<arch::assetManager::assets::Font>> FontLoader::LoadFromFile(
	const std::filesystem::path& path
) const noexcept {
	return std::optional<std::shared_ptr<arch::assetManager::assets::Font>>();
}

} // namespace arch::assetManager
