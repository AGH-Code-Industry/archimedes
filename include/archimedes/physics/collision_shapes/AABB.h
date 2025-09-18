#pragma once
#include "Shape.h"

namespace arch::physics {
	struct AABB : Shape {
		float3 topLeft;
		float3 bottomRight;

		AABB(float3 topLeft, float3 bottomRight) : topLeft(topLeft), bottomRight(bottomRight) {}
	};
} // namespace arch::physics