#pragma once

#include "math/Math.h"
#include "physics/components/MassCenter.h"

namespace arch::physics {

struct Moveable {
	MassCenter center;
	math::float2 force;
	math::float2 velocity;
};

}