#pragma once

namespace arch::physics {

struct RigidBodyComponent {
	f32 mass;
	float3 force;
	float3 velocity;
};

}