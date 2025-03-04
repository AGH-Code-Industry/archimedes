#include "physics/System.h"

#include "ecs/Domain.h"
#include "ecs/View.h"
#include "math/Math.h"
#include "physics/components/Colliding.h"
#include "physics/components/Moveable.h"

namespace arch::physics {

namespace {
[[nodiscard]] bool areColliding(const BBox& lhs, const BBox& rhs) {
	if (lhs.topLeft.x < rhs.topLeft.x && rhs.topLeft.x < lhs.bottomRight.x) {
		// top left corner of right rectangle is in the left rectangle
		if (lhs.bottomRight.y < rhs.topLeft.y && rhs.topLeft.y < lhs.topLeft.y) {
			return true;
		}

		// bottom left corner of right rectangle is in the left rectangle
		if (lhs.bottomRight.y < rhs.bottomRight.y && rhs.bottomRight.y < lhs.topLeft.y) {
			return true;
		}
	}

	if (lhs.topLeft.x < rhs.bottomRight.x && rhs.bottomRight.x < lhs.bottomRight.x) {
		// top right corner of right rectangle is in the left rectangle
		if (lhs.bottomRight.y < rhs.topLeft.y && rhs.topLeft.y < lhs.topLeft.y) {
			return true;
		}

		// bottom right corner of right rectangle is in the left rectangle
		if (lhs.bottomRight.y < rhs.bottomRight.y && rhs.bottomRight.y < lhs.topLeft.y) {
			return true;
		}
	}

	return false;
}
} // namespace

System::System(ecs::Domain& domain): _domain(domain), _prevTimePoint(Clock::now()) {}

f32 System::update() {
	auto viewPhysicsComponents = _domain.view<Moveable>();

	const Duration deltaTime = Clock::now() - _prevTimePoint;
	const f32 t = deltaTime.count();

	viewPhysicsComponents.forEach([&](ecs::Entity _, Moveable& e) {
		// update position
		e.center.position += e.velocity * t;

		// update speed
		const float2 a = e.force / e.center.mass;
		e.velocity += a * t;
	});

	_collisionDetection(t);

	_prevTimePoint = Clock::now();

	return t;
}

void System::_collisionDetection(f32 t) const {
	auto bboxesView = _domain.view<const BBox>();
	auto collidingView = _domain.view<Colliding>();

	// collide every Colliding object with all BBoxes
	collidingView.forEach([&](const ecs::Entity lhs, const Colliding& c) {
		bboxesView.forEach([&](const ecs::Entity rhs, const BBox& b) {
			if (areColliding(c.box, b)) {
				c.action(lhs, rhs);
			}
		});
	});

	// collide every Colliding with every other Colliding
	for (auto lhsIt = collidingView.begin(); lhsIt != collidingView.end(); ++lhsIt) {
		if (std::next(lhsIt) == collidingView.end()) {
			break;
		}

		const ecs::Entity lhs = *lhsIt;
		auto [c1] = collidingView.get(lhs);

		for (auto rhsIt = std::next(lhsIt); rhsIt != collidingView.end(); ++rhsIt) {
			const ecs::Entity rhs = *rhsIt;

			if (auto [c2] = collidingView.get(rhs); areColliding(c1.box, c2.box)) {
				c1.action(lhs, rhs);
			}
		}
	}
}

} // namespace arch::physics
