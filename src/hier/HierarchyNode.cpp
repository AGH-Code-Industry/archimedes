#include <ecs/EntityFormatter.h>
#include <hier/HierarchyNode.h>

namespace arch::hier {

HierarchyNode::HierarchyNode(const ecs::Entity entity) noexcept {
	this->_entity = entity;
}

void HierarchyNode::_unparent() noexcept {
	// unlink node from siblings
	_prev->_next = _next;
	_next->_prev = _prev;

	if ((_parent->_count--) == 1) {
		// only child -> reset children
		_parent->_first = (_parent->_last = nullptr);
	} else if (_prev->_entity == ecs::nullEntity) {
		// first child
		_parent->_first = _next;
	} else if (_next->_entity == ecs::nullEntity) {
		// last child
		_parent->_last = _prev;
	}

	// unlink hierarchy
	_next = (_prev = (_parent = nullptr));
	_depth = 0;
}

void HierarchyNode::_setParentUnchecked(Self& newParent) noexcept {
	_parent = &newParent;

	// add at the end
	newParent._sentinel._prev = this;
	_next = &newParent._sentinel;

	if (newParent._count == 0) {
		// 1st child
		newParent._sentinel._next = (newParent._first = this);
		_prev = &newParent._sentinel;
	} else {
		_prev = newParent._last;
		newParent._last->_next = this;
	}

	newParent._last = this;
	++newParent._count;
	_depth = newParent._depth + 1;
}

bool HierarchyNode::setParent(Self& newParent) noexcept {
	if (ancestry(newParent) > 0) {
		return false;
	}

	_unparent();
	_setParentUnchecked(newParent);

	return true;
}

bool HierarchyNode::isChildOf(const Self& parent) const noexcept {
	return _parent == &parent;
}

HierarchyNode& HierarchyNode::ancestor(u32 which) noexcept {
	auto node = this;
	while (which-- && node->_parent != nullptr) {
		node = node->_parent;
	}
	return *node;
}

const HierarchyNode& HierarchyNode::ancestor(u32 which) const noexcept {
	return const_cast<Self*>(this)->ancestor(which);
}

i64 HierarchyNode::ancestry(const Self& other) const noexcept {
	if (_depth < other._depth) {
		auto node = &other;
		const u32 steps = other._depth - _depth;
		for (u32 i = 0; i != steps && node; ++i) {
			node = node->_parent;
		}

		return node == this ? steps : 0;
	} else if (_depth == other._depth) {
		return 0;
	} else {
		return -other.ancestry(*this);
	}
}

HierarchyNode& HierarchyNode::parent() noexcept {
	return *_parent;
}

const HierarchyNode& HierarchyNode::parent() const noexcept {
	return *_parent;
}

HierarchyNode& HierarchyNode::child(u32 which) noexcept {
	ARCH_ASSERT(_count != 0, "childrenCount() == 0");

	auto node = _first;

	for (; which != 0 && node->_next->_entity != ecs::nullEntity; --which) {
		node = node->_next;
	}
	return *static_cast<Self*>(node);
}

const HierarchyNode& HierarchyNode::child(u32 which) const noexcept {
	return const_cast<Self*>(this)->child(which);
}

OptRef<HierarchyNode> HierarchyNode::childOpt(u32 which) noexcept {
	if (_count != 0) {
		return OptRef(child(which));
	}
	return std::nullopt;
}

OptRef<const HierarchyNode> HierarchyNode::childOpt(u32 which) const noexcept {
	if (_count != 0) {
		return OptRef(child(which));
	}
	return std::nullopt;
}

HierarchyNode& HierarchyNode::firstChild() noexcept {
	return *static_cast<HierarchyNode*>(_first);
}

const HierarchyNode& HierarchyNode::firstChild() const noexcept {
	return *static_cast<const HierarchyNode*>(_first);
}

OptRef<HierarchyNode> HierarchyNode::firstChildOpt() noexcept {
	if (_count != 0) {
		return OptRef(firstChild());
	}
	return std::nullopt;
}

OptRef<const HierarchyNode> HierarchyNode::firstChildOpt() const noexcept {
	if (_count != 0) {
		return OptRef(firstChild());
	}
	return std::nullopt;
}

HierarchyNode& HierarchyNode::lastChild() noexcept {
	return *static_cast<HierarchyNode*>(_last);
}

const HierarchyNode& HierarchyNode::lastChild() const noexcept {
	return *static_cast<const HierarchyNode*>(_last);
}

OptRef<HierarchyNode> HierarchyNode::lastChildOpt() noexcept {
	if (_count != 0) {
		return OptRef(lastChild());
	}
	return std::nullopt;
}

OptRef<const HierarchyNode> HierarchyNode::lastChildOpt() const noexcept {
	if (_count != 0) {
		return OptRef(lastChild());
	}
	return std::nullopt;
}

u32 HierarchyNode::childrenCount() const noexcept {
	return _count;
}

ecs::Entity HierarchyNode::entity() const noexcept {
	return _entity;
}

u32 HierarchyNode::depth() const noexcept {
	return _depth;
}

bool HierarchyNode::isRoot() const noexcept {
	return _parent == nullptr;
}

auto HierarchyNode::recursiveIterable() noexcept
	-> decltype(std::ranges::subrange(_details::HierarchyIterator(nullptr), _details::HierarchyIterator(nullptr))) {
	return std::ranges::subrange(_details::HierarchyIterator(_first), _details::HierarchyIterator(&_sentinel));
}

auto HierarchyNode::recursiveIterable() const noexcept
	-> decltype(std::ranges::subrange(_details::HierarchyIterator(nullptr), _details::HierarchyIterator(nullptr))) {
	return std::ranges::subrange(_details::HierarchyIterator(_first), _details::HierarchyIterator(&_sentinel));
}

HierarchyNode::Iterator HierarchyNode::begin() const noexcept {
	return ChildrenIterator(_count ? _first : &_sentinel);
}

HierarchyNode::Iterator HierarchyNode::cbegin() const noexcept {
	return begin();
}

HierarchyNode::Iterator HierarchyNode::end() const noexcept {
	return ChildrenIterator(&_sentinel);
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
