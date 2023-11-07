#include "ecs/component.h"
#include <glm/vec3.hpp>

namespace arch::ecs {

class Transform : Component {
public:
	Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : position(position), rotation(rotation), scale(scale) {}

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};

}