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

void TextureImporter::Import(const std::filesystem::path& sourcePath, const std::filesystem::path& processedPath) {
	if (!std::filesystem::exists(sourcePath)) {
		arch::Logger::error("File not found - '{}'", sourcePath.string());
		return;
	} else {
		arch::Logger::trace("File successfully located - '{}'", sourcePath.string());
	}

	TextureData image{};

	uint8_t* data{ stbi_load(sourcePath.string().c_str(), &image.width, &image.height, &image.channels, 0) };
	if (!data) {
		arch::Logger::error("Error reading PNG ('{}'): {}", sourcePath.string(), stbi_failure_reason());
		return;
	}

	if (image.channels < 4) {
		arch::Logger::warn("Missing alpha channel ('{}')", sourcePath.string());
		// TODO: Convert RGB to RGBA in case of missing alpha channel
	}

	image.pixels.assign(
		data,
		data +
			static_cast<size_t>(image.width) * static_cast<size_t>(image.height) * static_cast<size_t>(image.channels)
	);

	stbi_image_free(data);
	arch::Logger::trace("Pixels read successfully ('{}')", sourcePath.string());

	uint32_t maxMipMapLevels{ static_cast<uint32_t>(std::floor(std::log2(std::max(image.width, image.height)))) + 1 };

	if (_mipMapLevels > maxMipMapLevels && _mipMapLevels != 0) {
		arch::Logger::warn(
			"Declared amount of mipmaps to generate bigger than possible ('{}'). Generating as many as possible.",
			sourcePath.string()
		);
		_mipMapLevels = maxMipMapLevels;
	}

	std::vector<TextureData> mipMaps;
}

void TextureImporter::SetMipMapGenerationProperties(
	MipMapGenerationMethod genMethod,
	uint32_t mipMapLevels,
	uint32_t minAllowedDimension
) {
	_genMethod = genMethod;
	_mipMapLevels = mipMapLevels;
	_minAllowedDimension = minAllowedDimension;
}

} // namespace arch::assetManager
