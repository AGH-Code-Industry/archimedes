#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace arch {

struct Vertex {
  glm::vec3 position;
  glm::vec2 normal;
  glm::vec2 tex_coords;
};

}
