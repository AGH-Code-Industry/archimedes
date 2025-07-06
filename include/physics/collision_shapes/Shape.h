#pragma once
#include <Mmath.h>

namespace arch::physics {
	struct Shape {
	virtual ~Shape() = default;

	virtual void update(float3 velocity, f32 time) {}
	};
} // namespace arch::physics