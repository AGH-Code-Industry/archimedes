#pragma once

#include <functional>

#include "BBox.h"
#include "Moveable.h"
#include <ecs/Entity.h>

namespace arch::physics {

struct Colliding {
	BBox box;
	Moveable body;
	std::function<void(ecs::Entity, ecs::Entity)> action;
};

}
