#include "physics/System.h"

#include "ecs/View.h"
#include "physics/components/CollidingComponent.h"
#include "physics/components/MoveableComponent.h"

namespace arch::physics {

namespace {
[[nodiscard]] bool areColliding(const BBoxComponent& lhs, const BBoxComponent& rhs) {
	if (lhs.topLeft.x <= rhs.topLeft.x && rhs.topLeft.x <= lhs.bottomRight.x) {
		// top left corner of right rectangle is in the left rectangle
		if (lhs.bottomRight.y <= rhs.topLeft.y && rhs.topLeft.y <= lhs.topLeft.y) {
			return true;
		}

		// bottom left corner of right rectangle is in the left rectangle
		if (lhs.bottomRight.y <= rhs.bottomRight.y && rhs.bottomRight.y <= lhs.topLeft.y) {
			return true;
		}
	}

	if (lhs.topLeft.x <= rhs.bottomRight.x && rhs.bottomRight.x <= lhs.bottomRight.x) {
		// top right corner of right rectangle is in the left rectangle
		if (lhs.bottomRight.y <= rhs.topLeft.y && rhs.topLeft.y <= lhs.topLeft.y) {
			return true;
		}

		// bottom right corner of right rectangle is in the left rectangle
		if (lhs.bottomRight.y <= rhs.bottomRight.y && rhs.bottomRight.y <= lhs.topLeft.y) {
			return true;
		}
	}

	return false;
}
} // namespace

System::System(ecs::Domain& domain): _domain(domain), _prevTimePoint(Clock::now()) {}

f32 System::update() {
	auto viewPhysicsComponents = _domain.view<MoveableComponent>();
	auto viewColliding = _domain.view<CollidingComponent>();

	const Duration deltaTime = Clock::now() - _prevTimePoint;
	const f32 t = deltaTime.count();

	viewPhysicsComponents.forEach([&](MoveableComponent& e) {
		// update position
		e.center.position += e.velocity * t;

		// update speed
		const float2 a = e.force / e.center.mass;
		e.velocity += a * t;
	});

	viewColliding.forEach([&](CollidingComponent& c) {
		// update position
		c.body.center.position += c.body.velocity * t;
		c.box.topLeft += c.body.velocity * t;
		c.box.bottomRight += c.body.velocity * t;

		// update speed
		const float2 a = c.body.force / c.body.center.mass;
		c.body.velocity += a * t;
	});

	_collisionDetection(t);

	_prevTimePoint = Clock::now();

	return t;
}

void System::_collisionDetection(f32 t) const {
	auto viewBBoxes = _domain.view<const BBoxComponent>();
	auto viewColliding = _domain.view<CollidingComponent>();

	// collide every Colliding object with all BBoxes
	viewColliding.forEach([&](const ecs::Entity lhs, const CollidingComponent& c) {
		viewBBoxes.forEach([&](const ecs::Entity rhs, const BBoxComponent& b) {
			if (areColliding(c.box, b)) {
				c.action(lhs, rhs);
			}
		});
	});

	// collide every Colliding with every other Colliding
	for (auto lhsIt = viewColliding.begin(); lhsIt != viewColliding.end(); ++lhsIt) {
		if (std::next(lhsIt) == viewColliding.end()) {
			break;
		}

		const ecs::Entity lhs = *lhsIt;
		auto [c1] = viewColliding.get(lhs);

		for (auto rhsIt = std::next(lhsIt); rhsIt != viewColliding.end(); ++rhsIt) {
			const ecs::Entity rhs = *rhsIt;

			if (auto [c2] = viewColliding.get(rhs); areColliding(c1.box, c2.box)) {
				c1.action(lhs, rhs);
			}
		}
	}
}

} // namespace arch::physics
