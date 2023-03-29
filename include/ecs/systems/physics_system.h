#ifndef ECS_PHYSICS_SYSTEM_H
#define ECS_PHYSICS_SYSTEM_H

#include "ecs/game_coordinator.h"
#include "ecs/systems/behavior.h"
#include "ecs/components/transform.h"

namespace arch::ecs {
	class PhysicsSystem : public Behavior {
	public:
		void init() override;
		void update(float delta_time) override;
	};
}
#endif
