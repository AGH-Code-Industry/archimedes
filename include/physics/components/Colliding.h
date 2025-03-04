#pragma once

#include "BBox.h"
#include "math/Math.h"
#include "Moveable.h"

namespace arch::physics {

struct Colliding {
	BBox box;
	Moveable body;
};

}
