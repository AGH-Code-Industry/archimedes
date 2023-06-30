#pragma once

#include "resource/resource_loader.h"
#include "graphics/texture.h"
#include "exception.h"

namespace arch {

class TextureLoader : ResourceLoader<Texture> {
public:
    Texture read_file(const std::filesystem::path &filepath) override;
};

class TextureLoadingException : Exception {
public:
    TextureLoadingException(const std::filesystem::path &filepath, const std::string &reason);
};

}