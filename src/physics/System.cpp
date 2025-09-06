#include <archimedes/ecs/Domain.h>
#include <archimedes/ecs/View.h>
#include <archimedes/math/Math.h>
#include <archimedes/physics/System.h>
#include <archimedes/physics/RigidBodyComponent.h>
#include <archimedes/scene/components/TransformComponent.h>
#include <archimedes/physics/ColliderComponent.h>

namespace arch::physics {

System::System(ecs::Domain& domain): _domain(domain), _prevTimePoint(Clock::now()) {}

f32 System::update() {
	auto viewRigidBodies = _domain.view<RigidBodyComponent, scene::components::TransformComponent>();

	const Duration deltaTime = Clock::now() - _prevTimePoint;
	const f32 t = deltaTime.count();

	for (auto [entity, rigidBody, transform] : viewRigidBodies.all()) {
		// update position
		transform.position += rigidBody.velocity * t;

		// update speed
		const float3 a = rigidBody.force / rigidBody.mass;
		rigidBody.velocity += a * t;

		auto collider = _domain.tryGetComponent<ColliderComponent>(entity);

		if (collider.hasValue()) {
			std::visit([&](auto& shape) {
				shape.update(rigidBody.velocity, t);
			}, collider.get().shape);
		}

	}

	_collisionDetection(t);

	_prevTimePoint = Clock::now();

	return t;
}

void System::_collisionDetection(f32 t) const {
	auto viewColliding = _domain.view<ColliderComponent>();

	// collide every CollidingComponent with every other CollidingComponent
	for (auto lhsIt = viewColliding.begin(); lhsIt != viewColliding.end(); ++lhsIt) {
		if (std::next(lhsIt) == viewColliding.end()) {
			break;
		}

		const ecs::Entity lhs = *lhsIt;
		auto [c1] = viewColliding.get(lhs);

		for (auto rhsIt = std::next(lhsIt); rhsIt != viewColliding.end(); ++rhsIt) {
			const ecs::Entity rhs = *rhsIt;

			if (auto [c2] = viewColliding.get(rhs); ColliderComponent::areColliding(c1, c2)) {
				c1.action(lhs, rhs);
			}
		}
	}
}

} // namespace arch::physics
