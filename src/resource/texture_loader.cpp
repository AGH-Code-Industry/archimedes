#include "resource/texture_loader.h"

#include <sail-c++/sail-c++.h>
#include <sail-common/common.h>
#include <sail-common/error.h>
#include <spdlog/spdlog.h>

using namespace arch;

Texture TextureLoader::read_file(const std::filesystem::path &filepath) {
    if (!std::filesystem::exists(filepath))
        throw TextureLoadingException(filepath, "file does not exists");
    sail::image image(filepath);
    if (!image.is_valid()) {
        throw TextureLoadingException(filepath, "failed to read the file");
    }
    //TODO: check if more formats can be supported
    //iirc images assimp gives access to are always 32-bit RGBA
    if (image.pixel_format() != SAIL_PIXEL_FORMAT_BPP32_RGBA) {
        if (image.can_convert(SAIL_PIXEL_FORMAT_BPP32_RGBA)) {
            image.convert(SAIL_PIXEL_FORMAT_BPP32_RGBA);
        } else {
            throw TextureLoadingException(filepath, "cannot convert image to supported format");
        }
    }
    return Texture(image.width(), image.height(), image.pixels());
}

TextureLoadingException::TextureLoadingException(
    const std::filesystem::path &filepath,
    const std::string &reason
) : Exception("Texture loading") { 
    append_msg("from path ");
    append_msg(filepath);
    append_msg(" ");
    append_msg(reason);
}

