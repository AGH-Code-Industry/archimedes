#include "AudioImporter.h"

namespace arch::assetManager {
bool AudioImporter::SupportsFile(const std::filesystem::path& path) const {
	return false;
}

void AudioImporter::Import(const std::filesystem::path& sourcePath, const std::filesystem::path& processedPath) const {}

std::string AudioImporter::GetType() const {
	return std::string();
}
} // namespace arch::assetManager
