#pragma once

#include "BBox.h"
#include "math/Math.h"
#include "Movable.h"

namespace arch::physics {

struct Colliding {
	BBox box;
	float2 bottomRight;
};

}
