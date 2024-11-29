#pragma once

#include "ecs/Domain.h"

namespace arch::physics {

class System {
public:
	static void setDomain(ecs::Domain& domain);
	static void update();

private:
	inline static ecs::Domain* _domain = nullptr;
};

}