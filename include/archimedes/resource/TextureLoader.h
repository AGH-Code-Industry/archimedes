// will be useful in the future

// #pragma once
//
// #include "resource/ResourceLoader.h"
// #include "gfx/texture.h"
// #include "Exception.h"
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