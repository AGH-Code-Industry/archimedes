#include <archimedes/ecs/Domain.h>
#include <archimedes/ecs/View.h>
#include <archimedes/math/Math.h>
#include <archimedes/physics/PhysicsSystem.h>
#include <archimedes/physics/components/RigidBodyComponent.h>
#include <archimedes/scene/components/TransformComponent.h>

namespace arch::physics {

using TransformComponent = scene::components::TransformComponent;

PhysicsSystem::PhysicsSystem(ecs::Domain& domain):
	_domain(domain), _prevTimePoint(Clock::now()), _collisionSystem(domain) {}

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

	_collisionSystem.update();
	_prevTimePoint = Clock::now();

	return t;
}

std::vector<ecs::Entity> PhysicsSystem::getEnteredCollisions(ecs::Entity entity) const{
	return _collisionSystem.getEnteredCollisions(entity);
}

std::vector<ecs::Entity> PhysicsSystem::getExitedCollisions(ecs::Entity entity) const {
	return _collisionSystem.getExitedCollisions(entity);
}

std::vector<ecs::Entity> PhysicsSystem::getLastingCollisions(ecs::Entity entity) const {
	return _collisionSystem.getLastingCollisions(entity);
}

} // namespace arch::physics
