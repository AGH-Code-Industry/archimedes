// #pragma once
//
// #include "resource/resource_loader.h"
// #include "gfx/texture.h"
// #include "exception.h"
//
// namespace arch {
//
// class TextureLoader : public ResourceLoader<Texture> {
// public:
//     Texture read_file(const std::filesystem::path &filepath) override;
// };
//
// class TextureLoadingException : public Exception {
// public:
//     TextureLoadingException(const std::filesystem::path &filepath, const std::string &reason);
// };
//
// }