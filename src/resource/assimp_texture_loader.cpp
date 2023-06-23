#include "resource/assimp_texture_loader.h"

#include <assimp/Importer.hpp>

using namespace arch;

Texture AssimpTextureLoader::read_file(const std::filesystem::path &filepath) const {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filepath, 0);
    return {};
}