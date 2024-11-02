#pragma once

#include <ecs/Entity.h>

namespace arch::hier {

struct ChildNode {
	static constexpr bool inPlaceComponent = true;

	ecs::Entity entity;
	const ChildNode* prev;
	const ChildNode* next;
};

} // namespace arch::hier
