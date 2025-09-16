#pragma once

#include <archimedes/math/Math.h>

namespace arch::physics {

struct BBox {
	math::float2 topLeft;
	math::float2 bottomRight;
};

} // namespace arch::physics
