#include "resource/texture_loader.h"

namespace arch {

class AssimpTextureLoader : public TextureLoader {
public:
    Texture read_file(const std::filesystem::path &path) const override;
};

}