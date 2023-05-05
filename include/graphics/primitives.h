#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace arch {

using Index = unsigned int;

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 tex_coords;
};

template <typename Vec3>
glm::vec3 convert(Vec3 vec) {
    return glm::vec3 { vec.x, vec.y, vec.z };
}

template <typename Vec2>
glm::vec2 convert(Vec2 vec) {
    return glm::vec2 { vec.x, vec.y};
}

}
