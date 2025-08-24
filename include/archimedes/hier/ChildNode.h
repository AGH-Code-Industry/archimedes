#pragma once

#include <archimedes/ecs/EntityTraits.h>

namespace arch::hier {

struct HierarchyNode;

/// @brief Base class of HierarchyNode, also type of sentinel.
struct ChildNode {
	/// @brief Previous sibling
	ChildNode* _prev;
	/// @brief Next sibling
	ChildNode* _next;

	/// @brief Entity value
	ecs::Entity _entity = ecs::nullEntity;
};

} // namespace arch::hier
