#pragma once

#include "physics/components/MassCenterComponent.h"

namespace arch::physics {

struct MoveableComponent {
	MassCenterComponent center;
	float2 force;
	float2 velocity;
};

}