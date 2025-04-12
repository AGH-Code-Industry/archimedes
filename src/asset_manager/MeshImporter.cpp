#include "MeshImporter.h"

namespace arch::assetManager {
bool MeshImporter::SupportsFile(const std::filesystem::path& path) const {
	return false;
}

void MeshImporter::Import(const std::filesystem::path& sourcePath, const std::filesystem::path& processedPath) const {}

std::string MeshImporter::GetType() const {
	return std::string();
}
} // namespace arch::assetManager
