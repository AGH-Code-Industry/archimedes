#include <asset_manager/importers/TextureImporter.h>

namespace arch::assetManager {
bool TextureImporter::SupportsFile(const std::filesystem::path& path) const noexcept {
	std::string extension{ path.extension().string() };
	if (extension == ".png") {
		arch::Logger::trace("File format verified ('{}').", path.string());
		return true;
	} else {
		arch::Logger::warn("File format not supported ('{}').", path.string());
		return false;
	}
}

void TextureImporter::Import(
	const std::filesystem::path& sourcePath,
	const std::filesystem::path& processedPath,
	const std::filesystem::path& filePath
) const noexcept {

	// TODO: Convert textures to KTX
	std::filesystem::copy(sourcePath, processedPath, std::filesystem::copy_options::update_existing);
}

} // namespace arch::assetManager
