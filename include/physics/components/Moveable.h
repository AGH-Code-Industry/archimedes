#pragma once

#include "physics/components/MassCenter.h"

namespace arch::physics {

struct Moveable {
	MassCenter center;
	float2 force;
	float2 velocity;
};

}