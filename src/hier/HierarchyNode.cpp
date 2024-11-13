#include <hier/HierarchyNode.h>

namespace arch::hier {

HierarchyNode::HierarchyNode(const ecs::Entity entity) noexcept {
	this->entity = entity;
}

auto HierarchyNode::recursiveIterable() noexcept
	-> decltype(std::ranges::subrange(_details::HierarchyIterator(first), _details::HierarchyIterator(&sentinel))) {
	return std::ranges::subrange(_details::HierarchyIterator(first), _details::HierarchyIterator(&sentinel));
}

auto HierarchyNode::recursiveIterable() const noexcept
	-> decltype(std::ranges::subrange(_details::HierarchyIterator(first), _details::HierarchyIterator(&sentinel))) {
	return std::ranges::subrange(_details::HierarchyIterator(first), _details::HierarchyIterator(&sentinel));
}

HierarchyNode::Iterator HierarchyNode::begin() const noexcept {
	return ChildrenIterator(count ? first : &sentinel);
}

HierarchyNode::Iterator HierarchyNode::cbegin() const noexcept {
	return begin();
}

HierarchyNode::Iterator HierarchyNode::end() const noexcept {
	return ChildrenIterator(&sentinel);
}

HierarchyNode::Iterator HierarchyNode::cend() const noexcept {
	return end();
}

HierarchyNode::ReverseIterator HierarchyNode::rbegin() const noexcept {
	return std::make_reverse_iterator(end());
}

HierarchyNode::ReverseIterator HierarchyNode::crbegin() const noexcept {
	return std::make_reverse_iterator(end());
}

HierarchyNode::ReverseIterator HierarchyNode::rend() const noexcept {
	return std::make_reverse_iterator(begin());
}

HierarchyNode::ReverseIterator HierarchyNode::crend() const noexcept {
	return std::make_reverse_iterator(begin());
}

} // namespace arch::hier
