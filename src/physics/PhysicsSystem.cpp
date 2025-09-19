#include "archimedes/physics/Collisions.hpp"

#include <archimedes/ecs/Domain.h>
#include <archimedes/ecs/View.h>
#include <archimedes/math/Math.h>
#include <archimedes/physics/PhysicsSystem.h>
#include <archimedes/physics/components/ColliderComponent.h>
#include <archimedes/physics/components/RigidBodyComponent.h>
#include <archimedes/scene/components/TransformComponent.h>

namespace arch::physics {

using TransformComponent = scene::components::TransformComponent;

PhysicsSystem::PhysicsSystem(ecs::Domain& domain): _domain(domain), _prevTimePoint(Clock::now()) {}

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

	_collisionDetection(t);

	_prevTimePoint = Clock::now();

	return t;
}

void PhysicsSystem::_collisionDetection(f32 t) const {
	auto viewColliding = _domain.view<ColliderComponent, TransformComponent>();

	// collide every CollidingComponent with every other CollidingComponent
	for (auto lhsIt = viewColliding.begin(); lhsIt != viewColliding.end(); ++lhsIt) {
		if (std::next(lhsIt) == viewColliding.end()) {
			break;
		}

		const ecs::Entity lhs = *lhsIt;
		auto [collider1, transform1] = viewColliding.get(lhs);

		for (auto rhsIt = std::next(lhsIt); rhsIt != viewColliding.end(); ++rhsIt) {
			const ecs::Entity rhs = *rhsIt;

			auto [collider2, transform2] = viewColliding.get(rhs);
			if (ColliderComponent::areColliding(collider1, collider2, transform1, transform2)) {
				collider1.action(lhs, rhs);
			}
		}
	}
}

} // namespace arch::physics
