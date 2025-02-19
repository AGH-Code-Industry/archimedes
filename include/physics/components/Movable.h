#pragma once

#include "math/Math.h"
#include "physics/components/MassCenter.h"

namespace arch::physics {

struct Movable {
	MassCenter center;
	float2 force;
	float2 velocity;
};

}