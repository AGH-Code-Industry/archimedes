#include "ShaderImporter.h"

namespace arch::assetManager {
bool ShaderImporter::SupportsFile(const std::filesystem::path& path) const {
	return false;
}

void ShaderImporter::Import(const std::filesystem::path& sourcePath, const std::filesystem::path& processedPath) const {
}

std::string ShaderImporter::GetType() const {
	return std::string();
}
} // namespace arch::assetManager
