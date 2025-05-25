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
		stbi_image_free(data);
		data = stbi_load(sourcePath.string().c_str(), &image.width, &image.height, &image.channels, STBI_rgb_alpha);
		if (!data) {
			arch::Logger::error("Error reloading PNG as RGBA ('{}'): {}", sourcePath.string(), stbi_failure_reason());
			return;
		}
		image.channels = 4;
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
	mipMaps.emplace_back(image);

	TextureData currMipSrc{ image };

	for (size_t level{ 1 }; level < _mipMapLevels; ++level) {
		size_t nextWidth{ std::max(1, currMipSrc.width / 2) };
		size_t nextHeight{ std::max(1, currMipSrc.height / 2) };

		if (((nextWidth < _minAllowedDimension || nextHeight < _minAllowedDimension) && _minAllowedDimension > 1) ||
			(currMipSrc.width == 1 && currMipSrc.height == 1)) {
			break;
		}

		if (_minAllowedDimension > 1 && (nextWidth < _minAllowedDimension || nextHeight < _minAllowedDimension)) {
			bool stop = false;
			if (nextWidth < _minAllowedDimension && currMipSrc.width >= _minAllowedDimension) {
				stop = true;
			}
			if (nextHeight < _minAllowedDimension && currMipSrc.height >= _minAllowedDimension) {
				stop = true;
			}
			if (stop) {
				break;
			}
		}

		TextureData nextMipLevel;
		nextMipLevel.width = nextWidth;
		nextMipLevel.height = nextHeight;
		nextMipLevel.channels = currMipSrc.channels;
		nextMipLevel.pixels.resize(
			static_cast<size_t>(nextMipLevel.width) * nextMipLevel.height * nextMipLevel.channels
		);

		int alphaChannelIndex = (nextMipLevel.channels == 4 || nextMipLevel.channels == 2) ?
			(nextMipLevel.channels - 1) :
			STBIR_ALPHA_CHANNEL_NONE;
		if (nextMipLevel.channels == 2) {
			alphaChannelIndex = 1;
		}

		// TODO: apply _genMethod
		stbir_filter filterMethod;

		int resizeResult{ stbir_resize_uint8_generic(
			currMipSrc.pixels.data(),
			currMipSrc.width,
			currMipSrc.height,
			0,
			nextMipLevel.pixels.data(),
			nextMipLevel.width,
			nextMipLevel.height,
			0,
			nextMipLevel.channels,
			alphaChannelIndex,
			0,
			STBIR_EDGE_CLAMP,
			filterMethod,
			STBIR_COLORSPACE_SRGB,
			NULL
		) };

		if (resizeResult == 0) {
			arch::Logger::error(
				"Mipmap level generation failed ('{}'): {}",
				sourcePath.string(),
				stbi_failure_reason()
			);
		}

		mipMaps.emplace_back(nextMipLevel);
		currMipSrc = nextMipLevel;

		if (currMipSrc.width == 1 && currMipSrc.height == 1) {
			break;
		}
	}

	// TODO: Handle generated mipmaps
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
