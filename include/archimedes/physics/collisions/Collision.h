#pragma once

namespace arch::physics {
enum CollisionState {
	Entered, ///< collision recently detected
	Exited, ///< collision recently disappeared
	Lasting, ///< collision have been existing for at least 2 frames
	CurrentlyFound ///< used by collision detection algorithms, those don't know current collision state
};

/// @brief Represents a collision detected by an algorithm (or stored in the system)
struct Collision {
	/// @brief Direction along which you have to push the objects so they don't collide
	float2 normal;
	
	/// @brief Minimal distance an object has to be pushed to exit the collision
	/// @warning It will still be detected probably, so you need to add a small number to it
	f32 depth;

	/// @brief State of the collision
	CollisionState state;

	/// @brief Helper factory method for disappearing collisions
	inline static Collision exitedCollision() {
		return {
			{ 0.0f, 0.0f },
			0.0f,
			CollisionState::Exited
		};
	}

	/// @brief Helper factory method
	inline Collision changedState(CollisionState newState) const { 
		return { normal, depth, newState };
	}
};

}
