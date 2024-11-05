#pragma once

#include <ranges>

#include "Children.h"
#include "HierarchyIterator.h"
#include <ecs/ComponentTraits.h>

namespace arch::hier {

struct HierarchyNode: public ChildNode, public Children {
	static constexpr bool inPlaceComponent = true;

	const HierarchyNode* parent = nullptr;

	auto recursiveIterable(
	) noexcept -> decltype(std::ranges::subrange(HierarchyIterator(first), HierarchyIterator(&sentinel)));
	auto recursiveIterable(
	) const noexcept -> decltype(std::ranges::subrange(HierarchyIterator(first), HierarchyIterator(&sentinel)));
};

} // namespace arch::hier
