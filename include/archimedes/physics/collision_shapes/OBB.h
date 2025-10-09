#pragma once
#include "Shape.h"
#include <archimedes/physics/PhysicsException.h>

namespace arch::physics {
struct OBB : Shape {
	float3 topLeft;
	float3 bottomRight;
	f32 rotation;

	OBB(float3 topLeft, float3 bottomRight, f32 rotation) : topLeft(topLeft), bottomRight(bottomRight),
		rotation(rotation) {
		if (topLeft.x > bottomRight.x || topLeft.y < bottomRight.y) {
			throw PhysicsException("Corners' coordinates don't match, consider swapping them");
		}
	}
};
} // namespace arch::physics