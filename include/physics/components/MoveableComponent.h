#pragma once

namespace arch::physics {

struct MoveableComponent {
	f32 mass;
	float3 force;
	float3 velocity;
};

}