#include "resource/resource_loader.h"
#include "graphics/texture.h"

namespace arch {

class TextureLoader : ResourceLoader<Texture> {
public:
    Texture read_file(const std::filesystem::path &filepath) override;
};

}