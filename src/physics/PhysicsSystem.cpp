#include <archimedes/ecs/Domain.h>
#include <archimedes/ecs/View.h>
#include <archimedes/math/Math.h>
#include <archimedes/physics/PhysicsSystem.h>
#include <archimedes/physics/components/RigidBodyComponent.h>
#include <archimedes/scene/components/TransformComponent.h>
#include <GLFW/glfw3.h>

namespace arch::physics {

using TransformComponent = scene::components::TransformComponent;

PhysicsSystem::PhysicsSystem(ecs::Domain& domain, f32 windowWidth, f32 windowHeight):
	_domain(domain),
	_prevTimePoint(Clock::now()),
	_collisionSystem(domain),
	_windowWidth(windowWidth),
	_windowHeight(windowHeight) {}

f32 PhysicsSystem::update() {
	auto viewRigidBodies = _domain.view<RigidBodyComponent, TransformComponent>();

	const Duration deltaTime = Clock::now() - _prevTimePoint;
	const f32 t = deltaTime.count();

	for (auto [entity, rigidBody, transform] : viewRigidBodies.all()) {
		// update position
		transform.position += rigidBody.linearVelocity * t;

		// update rotation
		transform.rotation *= quaternion(rigidBody.angularVelocity * t);
		transform.rotation = glm::normalize(transform.rotation);

		// update speed
		const float3 a = rigidBody.force / rigidBody.mass;
		rigidBody.linearVelocity += a * t;
	}

	float3 mousePosition = getMousePositionOnMap();
	_collisionSystem.update(mousePosition);
	_prevTimePoint = Clock::now();

	return t;
}

std::unordered_map<ecs::Entity, Collision> PhysicsSystem::getEnteredCollisions(ecs::Entity entity) const {
	return _collisionSystem.getEnteredCollisions(entity);
}

std::unordered_map<ecs::Entity, Collision> PhysicsSystem::getExitedCollisions(ecs::Entity entity) const {
	return _collisionSystem.getExitedCollisions(entity);
}

std::unordered_map<ecs::Entity, Collision> PhysicsSystem::getCollisions(ecs::Entity entity) const {
	return _collisionSystem.getCollisions(entity);
}

bool PhysicsSystem::hasMouseEntered(ecs::Entity entity) const {
	return _collisionSystem.hasMouseEntered(entity);
}

bool PhysicsSystem::hasMouse(ecs::Entity entity) const {
	return _collisionSystem.hasMouse(entity);
}

bool PhysicsSystem::hasMouseExited(ecs::Entity entity) const {
	return _collisionSystem.hasMouseExited(entity);
}

/// TODO: use the camera's data instead of hard coding window sizes
float3 PhysicsSystem::getMousePositionOnMap() const {
	float2 mousePos = input::Mouse::pos();
	mousePos.x = glm::mix(-1.0f, 1.0f, mousePos.x / _windowWidth);
	mousePos.y = glm::mix(-1.0f, 1.0f, mousePos.y / _windowHeight);
	return float3(mousePos.x, mousePos.y, 0.0f);

} // namespace arch::physics
}