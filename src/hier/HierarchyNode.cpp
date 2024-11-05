#include <hier/HierarchyNode.h>

namespace arch::hier {

auto HierarchyNode::recursiveIterable(
) noexcept -> decltype(std::ranges::subrange(HierarchyIterator(first), HierarchyIterator(&sentinel))) {
	return std::ranges::subrange(HierarchyIterator(first), HierarchyIterator(&sentinel));
}

auto HierarchyNode::recursiveIterable(
) const noexcept -> decltype(std::ranges::subrange(HierarchyIterator(first), HierarchyIterator(&sentinel))) {
	return std::ranges::subrange(HierarchyIterator(first), HierarchyIterator(&sentinel));
}

} // namespace arch::hier
