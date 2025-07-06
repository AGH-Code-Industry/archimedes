#pragma once

#include <functional>

#include "BBoxComponent.h"
#include "RigidBodyComponent.h"
#include <ecs/Entity.h>

namespace arch::physics {

struct CollidingComponent {
	BBoxComponent box;
	RigidBodyComponent body;
	std::function<void(ecs::Entity, ecs::Entity)> action;
};

}
