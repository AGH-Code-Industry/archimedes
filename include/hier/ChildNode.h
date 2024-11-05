#pragma once

#include <ecs/EntityTraits.h>

namespace arch::hier {

struct HierarchyNode;

struct ChildNode {
	const ChildNode* prev;
	const ChildNode* next;

	ecs::Entity entity = ecs::nullEntity;
};

} // namespace arch::hier
