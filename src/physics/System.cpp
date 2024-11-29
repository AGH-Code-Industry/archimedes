#include "ecs/View.h"
#include "physics/System.h"
#include "physics/components/Force.h"
#include "physics/components/Mass.h"
#include "physics/components/Velocity.h"

namespace arch::physics {

void System::setDomain(ecs::Domain& domain) {
	_domain = &domain;
}

void System::update() {
	if (!_domain) {
		throw std::runtime_error("System does not have a domain");
	}
	auto viewPhysicsComponents = _domain->view<const Force, const Mass, Velocity>();
	for (const ecs::Entity entity : viewPhysicsComponents) {
		auto compTuple = viewPhysicsComponents.get(entity);
		auto& f = std::get<0>(compTuple);
		auto& m = std::get<1>(compTuple);
		auto& v = std::get<2>(compTuple);

		auto a = f.value / m.mass;

		static constexpr float deltaTime = 1.0f;
		v.value += a * deltaTime;
	}
}

}
