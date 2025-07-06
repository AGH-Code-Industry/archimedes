#pragma once

#include <functional>

#include "BBoxComponent.h"
#include "MoveableComponent.h"
#include <ecs/Entity.h>

namespace arch::physics {

struct CollidingComponent {
	BBoxComponent box;
	MoveableComponent body;
	std::function<void(ecs::Entity, ecs::Entity)> action;
};

}
