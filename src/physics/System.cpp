#include "physics/System.h"

#include "ecs/Domain.h"
#include "ecs/View.h"
#include "math/Math.h"
#include "physics/components/Movable.h"

namespace arch::physics {

System::System(ecs::Domain& domain): _domain(domain), _prevTimePoint(Clock::now()) {}

f32 System::update() {
	auto viewPhysicsComponents = _domain.view<Movable>();

	const Duration deltaTime = Clock::now() - _prevTimePoint;
	const f32 t = deltaTime.count();

	for (const ecs::Entity entity : viewPhysicsComponents) {
		auto [e] = viewPhysicsComponents.get(entity);

		// update position
		e.center.position += e.velocity * t;

		// update speed
		auto a = e.force / e.center.mass;
		e.velocity += a * t;
	}

	_prevTimePoint = Clock::now();

	return t;
}

} // namespace arch::physics
