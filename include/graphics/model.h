#pragma once

#include "graphics/primitives.h"
#include "graphics/texture.h"

#include <vector>

namespace arch {

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};

struct Model {
    std::vector<Mesh> meshes;
};

}