#pragma once

#include "Children.h"

namespace arch::hier {

struct HierarchyNode: public ChildNode, public Children {
	const HierarchyNode* parent = nullptr;
};

} // namespace arch::hier
