#include "physics/System.h"

#include "ecs/Domain.h"
#include "ecs/View.h"
#include "math/Math.h"
#include "physics/components/Colliding.h"
#include "physics/components/Moveable.h"

namespace arch::physics {

System::System(ecs::Domain& domain): _domain(domain), _prevTimePoint(Clock::now()) {}

f32 System::update() {
	auto viewPhysicsComponents = _domain.view<Moveable>();

	const Duration deltaTime = Clock::now() - _prevTimePoint;
	const f32 t = deltaTime.count();

	viewPhysicsComponents.forEach([&](const ecs::Entity _, Moveable& e) {
		// update position
		e.center.position += e.velocity * t;

		// update speed
		const float2 a = e.force / e.center.mass;
		e.velocity += a * t;
	});

	collisionDetection(t);

	_prevTimePoint = Clock::now();

	return t;
}

void System::collisionDetection(f32 t) {
	auto collidingView = _domain.view<const Colliding>();
	// collide every Colliding object with all BBoxes

	// collide every Colliding with every other Colliding
}

} // namespace arch::physics
