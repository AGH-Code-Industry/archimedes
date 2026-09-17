#include <GLFW/glfw3.h>
#include <archimedes/Ecs.h>
#include <archimedes/math/Math.h>
#include <archimedes/physics/PhysicsSystem.h>
#include <archimedes/physics/components/RigidBodyComponent.h>
#include <archimedes/scene/components/TransformComponent.h>

namespace arch::physics {

using TransformComponent = scene::components::TransformComponent;

PhysicsSystem::PhysicsSystem(ecs::Domain& domain):
	_domain(domain),
	_prevTimePoint(Clock::now()),
	_collisionSystem(domain),
	_camera(std::nullopt) {}

PhysicsSystem::PhysicsSystem(ecs::Domain& domain, const Camera& camera):
	_domain(domain),
	_prevTimePoint(Clock::now()),
	_collisionSystem(domain),
	_camera(camera) {}

f32 PhysicsSystem::update() {
	auto viewRigidBodies = _domain.view<RigidBodyComponent, TransformComponent>();

	const Duration deltaTime = Clock::now() - _prevTimePoint;
	const f32 t = deltaTime.count();

	for (auto [rigidBody, transform] : viewRigidBodies.comps()) {
		// update position
		transform.position += float3(rigidBody.linearVelocity, 0.0f) * t;

		// update rotation
		transform.rotation *= quaternion(rigidBody.angularVelocity * t);
		transform.rotation = glm::normalize(transform.rotation);

		// update speed
		const float2 a = rigidBody.force / rigidBody.mass;
		rigidBody.linearVelocity += a * t;
	}

	float2 mousePosition{};
	if (_camera) {
		mousePosition = _camera.get().screenToWorldPos(input::Mouse::pos());
	}
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

} // namespace arch::physics
