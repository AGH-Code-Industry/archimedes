#pragma once

#include <ecs/EntityTraits.h>

namespace arch::hier {

struct HierarchyNode;

/// @brief Base class of HierarchyNode, also type of sentinel.
struct ChildNode {
	/// @brief Previous sibling
	ChildNode* prev;
	/// @brief Next sibling
	ChildNode* next;

	/// @brief Entity value
	ecs::Entity entity = ecs::nullEntity;
};

} // namespace arch::hier
