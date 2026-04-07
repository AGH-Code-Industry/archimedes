#pragma once

namespace arch::physics {
enum CollisionState {
	Entered,
	Exited,
	Lasting,
	CurrentlyFound
};

struct Collision {
	float3 normal;
	f32 depth;
	CollisionState state;

	inline static Collision exitedCollision() {
		return {
			{ 0.0f, 0.0f, 0.0f },
			0.0f,
			CollisionState::Exited
		};
	}

	inline Collision changeState(CollisionState newState) { return { normal, depth, newState };
	}
};

}
