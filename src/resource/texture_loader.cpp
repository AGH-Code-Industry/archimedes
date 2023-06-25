#include "resource/texture_loader.h"

#include <sail-c++/sail-c++.h>
#include <sail-common/common.h>
#include <sail-common/error.h>
#include <spdlog/spdlog.h>

using namespace arch;

Texture TextureLoader::read_file(const std::filesystem::path &filepath) {
    sail::image image(filepath);
    if (!image.is_valid()) {
        spdlog::error("Failed to load image {}", filepath.string());
        return {};
    }
    if (image.pixel_format() != SAIL_PIXEL_FORMAT_BPP32_RGBA) {
        if (image.can_convert(SAIL_PIXEL_FORMAT_BPP32_RGBA)) {
            image.convert(SAIL_PIXEL_FORMAT_BPP32_RGBA);
        } else {
            spdlog::error("Failed to convert image {} to RGBA format", filepath.string());
        }
    }
    return Texture(image.width(), image.height(), image.pixels());
}