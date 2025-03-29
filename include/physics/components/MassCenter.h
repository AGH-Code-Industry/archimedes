#pragma once

#include "math/Math.h"
#include "physics/components/Mass.h"

namespace arch::physics {

struct MassCenter {
	math::f32 mass;
	math::float2 position;
};

}

