#pragma once

#include <chrono>

#include <archimedes/ecs/Domain.h>
#include <archimedes/physics/collisions/CollisionSystem.h>

namespace arch::physics {

/// @brief Manages the physics on the scene, uses subsystems as modules
class PhysicsSystem {
public:
	using Clock = std::chrono::high_resolution_clock;
	using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;
	using Duration = std::chrono::duration<f32>;

	/// @brief Get shape-shape collisions that appeared in current frame
	std::unordered_map<ecs::Entity, Collision> getEnteredCollisions(ecs::Entity entity) const;

	/// @brief Get shape-shape collisions that disappeared in current frame
	std::unordered_map<ecs::Entity, Collision> getExitedCollisions(ecs::Entity entity) const;

	/// @brief Get shape-shape collisions (all of them)
	std::unordered_map<ecs::Entity, Collision> getCollisions(ecs::Entity entity) const;

	/// @brief Check if mouse collided with the shape in current frame
	bool hasMouseEntered(ecs::Entity entity) const;

	/// @brief Check if mouse collides with the shape (doesn't matter for how long)
	bool hasMouse(ecs::Entity entity) const;

	/// @brief Check if mouse stopped colliding with the shape in current frame
	bool hasMouseExited(ecs::Entity entity) const;

	/// @brief Calculate real mouse position (not on screen but on the map)
	float2 getMousePositionOnMap() const;

	PhysicsSystem(ecs::Domain& domain, f32 windowWidth, f32 windowHeight);

	/// @brief Update the system. Use it each frame.
	f32 update();

private:
	ecs::Domain& _domain;
	TimePoint _prevTimePoint;
	CollisionSystem _collisionSystem;
	f32 _windowWidth;
	f32 _windowHeight;

};

} // namespace arch::physics
