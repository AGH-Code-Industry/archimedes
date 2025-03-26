#pragma once

#include <functional>

#include "BBox.h"
#include "Moveable.h"
#include <ecs/Entity.h>

namespace arch::physics {

struct Colliding {
	std::function<void(ecs::Entity, ecs::Entity)> action;
	Moveable body;
	BBox box;
};

} // namespace arch::physics
