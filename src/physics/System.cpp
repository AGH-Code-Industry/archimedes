#include "physics/System.h"

namespace arch::physics {

void System::update() {
	if (!_domain) {
		throw std::runtime_error("System does not have a domain");
	}
	// auto viewPhysicsComponents = _domain->view<>;
}

}
