#pragma once

#include <chrono>

#include <archimedes/ecs/Domain.h>
#include <archimedes/physics/collisions/CollisionSystem.h>

namespace arch::physics {

class PhysicsSystem {
public:
	using Clock = std::chrono::high_resolution_clock;
	using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;
	using Duration = std::chrono::duration<f32>;

	std::vector<ecs::Entity> getEnteredCollisions(ecs::Entity entity) const;
	std::vector<ecs::Entity> getExitedCollisions(ecs::Entity entity) const;
	std::vector<ecs::Entity> getCollisions(ecs::Entity entity) const;

	bool hasMouseEntered(ecs::Entity entity) const;
	bool hasMouse(ecs::Entity entity) const;
	bool hasMouseExited(ecs::Entity entity) const;

	float3 getMousePositionOnMap() const;

	PhysicsSystem(ecs::Domain& domain, f32 windowWidth, f32 windowHeight);
	f32 update();

private:
	ecs::Domain& _domain;
	TimePoint _prevTimePoint;
	CollisionSystem _collisionSystem;
	f32 _windowWidth;
	f32 _windowHeight;

};

} // namespace arch::physics
