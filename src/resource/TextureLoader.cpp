// #include "resource/TextureLoader.h"
//
// #define STB_IMAGE_IMPLEMENTATION
// #include <stb_image.h>
// #include <spdlog/spdlog.h>
//
// using namespace arch;
//
// Texture TextureLoader::read_file(const std::filesystem::path &filepath) {
//     if (!std::filesystem::exists(filepath))
//         throw TextureLoadingException(filepath, "file does not exists");
//     int width, height, n_channels;
//     stbi_set_flip_vertically_on_load(true);
//     void *data = stbi_load(filepath.string().c_str(), &width, &height, &n_channels, STBI_rgb_alpha);
//     if (data == nullptr) {
//         throw TextureLoadingException(filepath, stbi_failure_reason());
//     }
//     spdlog::debug("Loaded image {}x{} {} with {} channels", width, height, filepath.string(), n_channels);
//     Texture texture(width, height, data);
//     stbi_image_free(data);
//     return texture;
// }
//
// TextureLoadingException::TextureLoadingException(
//     const std::filesystem::path &filepath,
//     const std::string &reason
// ) : Exception("Texture loading") {
//     append_msg("from path ");
//     append_msg(filepath.string());
//     append_msg(" - ");
//     append_msg(reason);
// }
//
