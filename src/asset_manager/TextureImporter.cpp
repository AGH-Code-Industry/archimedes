#include <asset_manager/TextureImporter.h>

namespace arch::assetManager {
bool TextureImporter::SupportsFile(const std::filesystem::path& path) const {
	return false;
}

void TextureImporter::Import(const std::filesystem::path& sourcePath, const std::filesystem::path& processedPath)
	const {}

} // namespace arch::assetManager
