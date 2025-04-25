#include <asset_manager/importers/ShaderImporter.h>

namespace arch::assetManager {
bool ShaderImporter::SupportsFile(const std::filesystem::path& path) const {
	return false;
}

void ShaderImporter::Import(const std::filesystem::path& sourcePath, const std::filesystem::path& processedPath) const {
}

} // namespace arch::assetManager
