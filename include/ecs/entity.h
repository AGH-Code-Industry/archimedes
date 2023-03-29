#ifndef ECS_ENTITY_H
#define ECS_ENTITY_H

#include <bitset>
#include "common.h"

namespace arch::ecs {
	class Entity {
	public:
		Entity();
		Entity(EntityId id, ComponentMask mask);

		ComponentMask mask;
		EntityId id;
	};
}

#endif
