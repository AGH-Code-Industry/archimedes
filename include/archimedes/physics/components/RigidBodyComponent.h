#pragma once

namespace arch::physics {

struct RigidBodyComponent {
	f32 mass;
	float2 force;
	float2 linearVelocity;
	f32 angularVelocity;
};

} // namespace arch::physics
