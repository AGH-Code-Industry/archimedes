#pragma once

#include "math/Math.h"

namespace arch::physics {

struct BBox {
	math::float2 topLeft;
	math::float2 bottomRight;
};

}
