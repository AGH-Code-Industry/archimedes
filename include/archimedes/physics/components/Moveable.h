#pragma once

#include <archimedes/math/Math.h>
#include <archimedes/physics/components/MassCenter.h>

namespace arch::physics {

struct Moveable {
	MassCenter center;
	math::float2 force;
	math::float2 velocity;
};

} // namespace arch::physics
