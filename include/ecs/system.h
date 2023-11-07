#ifndef ECS_SYSTEM_H
#define ECS_SYSTEM_H

#include <unordered_set>
#include "entity.h"
#include "common.h"

namespace arch::ecs {

class GameCoordinator;

class System {
public:
	virtual void init() = 0;

	ComponentMask mask;
	std::unordered_set<EntityId> entity_ids;
};

}
#endif
