#include <asset_manager/importers/TextureImporter.h>

namespace arch::assetManager {
bool TextureImporter::SupportsFile(const std::filesystem::path& path) const {
	std::string extension{ path.extension().string() };
	if (extension == ".png") {
		arch::Logger::trace("File format verified ('{}').", path.string());
		return true;
	} else {
		arch::Logger::warn("File format not supported ('{}').", path.string());
		return false;
	}
}

void TextureImporter::Import(const std::filesystem::path& sourcePath, const std::filesystem::path& processedPath)
	const {
	if (!std::filesystem::exists(sourcePath)) {
		arch::Logger::error("File not found - '{}'", sourcePath.string());
		return;
	} else {
		arch::Logger::trace("File succesfully located - '{}'", sourcePath.string());
	}

	// TODO: Read texture dimensions and channels
	int imageWidth{}, imageHeight{}, channels{};

	uint8_t* data{ stbi_load(sourcePath.string().c_str(), &imageWidth, &imageHeight, &channels, 0) };
	if (!data) {
		arch::Logger::error("Error reading PNG ('{}'): {}", sourcePath.string(), stbi_failure_reason());
		return;
	}

	if (channels < 4) {
		arch::Logger::warn("Missing alpha channel ('{}')", sourcePath.string());
		// TODO: Convert RGB to RGBA in case of missing alpha channel
	}

	std::vector<uint8_t> pixels{};
	pixels.assign(
		data,
		data + static_cast<size_t>(imageWidth) * static_cast<size_t>(imageHeight) * static_cast<size_t>(channels)
	);

	stbi_image_free(data);
	arch::Logger::trace("Pixels read succesfully ('{}')", sourcePath.string());
}

} // namespace arch::assetManager
