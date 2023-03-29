#ifndef ECS_BEHAVIOR_H
#define ECS_BEHAVIOR_H

#include "ecs/system.h"
namespace arch::ecs {
	class Behavior: public System {
	public:
		virtual void update(float deltaTime) = 0;
	};
}


#endif
