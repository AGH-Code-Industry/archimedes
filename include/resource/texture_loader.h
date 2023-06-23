#include "graphics/texture.h"

#include <filesystem>

namespace arch {

class TextureLoader {
public:
    virtual Texture read_file(const std::filesystem::path &path) const;
};

}
