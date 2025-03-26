#include "physics/System.h"

#include "ecs/Domain.h"
#include "ecs/EntityFormatter.h"
#include "ecs/View.h"
#include "math/Math.h"
#include "physics/components/Colliding.h"
#include "physics/components/Moveable.h"

namespace arch::physics {

namespace {
[[nodiscard]] bool areColliding(const BBox& lhs, const BBox& rhs) {
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
	auto viewPhysicsComponents = _domain.view<Moveable>();
	auto viewColliding = _domain.view<Colliding>();

	const Duration deltaTime = Clock::now() - _prevTimePoint;
	const f32 t = deltaTime.count();

	viewPhysicsComponents.forEach([&](Moveable& e) {
		// update position
		e.center.position += e.velocity * t;

		// update speed
		const float2 a = e.force / e.center.mass;
		e.velocity += a * t;
	});

	viewColliding.forEach([&](Colliding& c) {
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
	auto viewBBoxes = _domain.view<const BBox>();
	auto viewColliding = _domain.view<Colliding>();

	// collide every Colliding object with all BBoxes
	viewColliding.forEach([&](const ecs::Entity lhs, const Colliding& c) {
		viewBBoxes.forEach([&](const ecs::Entity rhs, const BBox& b) {
			/*Logger::debug(
				"({}, {}) ({}, {})",
				c.box.topLeft.x,
				c.box.topLeft.y,
				c.box.bottomRight.x,
				c.box.bottomRight.y
			);
			Logger::debug("({}, {}) ({}, {})", b.topLeft.x, b.topLeft.y, b.bottomRight.x, b.bottomRight.y);
			Logger::debug("");*/
			if (areColliding(c.box, b) || areColliding(b, c.box)) {
				// Logger::debug("collision");
				c.action(lhs, rhs);
			}
		});
	});

	// collide every Colliding with every other Colliding
	/*for (auto lhsIt = viewColliding.begin(); lhsIt != viewColliding.end(); ++lhsIt) {
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
	}*/
}

} // namespace arch::physics
