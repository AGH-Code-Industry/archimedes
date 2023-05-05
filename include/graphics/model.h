#ifndef ARCHIMEDES_MODEL_H
#define ARCHIMEDES_MODEL_H

#include "graphics/primitives.h"
#include "graphics/texture.h"

#include <vector>

namespace arch {

using Index = unsigned int;

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<Index> indices;
    std::vector<TextureId> textures;
};

struct Model {
    std::vector<Mesh> meshes;
};

}


#endif //ARCHIMEDES_MODEL_H
