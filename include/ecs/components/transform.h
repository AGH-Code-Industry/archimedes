#ifndef ECS_TRANSFORM_H
#define ECS_TRANSFORM_H


#include "ecs/component.h"

namespace arch::ecs {
	struct vec3 {
	public:
		float x, y, z;
	};

	class Transform : Component {
	public:
		Transform(vec3 position, vec3 rotation, vec3 scale) : position(position), rotation(rotation), scale(scale) {}

		vec3 position;
		vec3 rotation;
		vec3 scale;
	};
}


#endif
