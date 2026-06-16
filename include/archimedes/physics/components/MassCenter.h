#pragma once

#include <archimedes/math/Math.h>
#include <archimedes/physics/components/Mass.h>

namespace arch::physics {

struct MassCenter {
	math::f32 mass;
	math::float2 position;
};

} // namespace arch::physics

