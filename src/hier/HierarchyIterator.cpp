#include <hier/HierarchyNode.h>

// is sentinel?
#define SENT(x) ((x)->_entity == ::arch::ecs::nullEntity)
// is root?
#define ROOT(x) ((x)->_parent == nullptr)
// cast ChildNode* x to HierarchyNode*, used only if !SENT(x)
#define NODE(x) (static_cast<const ::arch::hier::HierarchyNode*>((x)))

namespace arch::hier::_details {

HierarchyIterator::HierarchyIterator(const ChildNode* node) noexcept: _current{ node } {}

ecs::Entity HierarchyIterator::operator*() const noexcept {
	ARCH_ASSERT(!SENT(_current) && !ROOT(NODE(_current)), "HierarchyIterator cannot dereference sentinels or roots");
	return _current->_entity;
}

HierarchyIterator& HierarchyIterator::operator++() noexcept {
	if (!SENT(_current)) {
		auto currentHierNode = NODE(_current);
		if (currentHierNode->_first != nullptr) {
			// jump to _first child
			_current = currentHierNode->_first;
			return *this;
		} else if (SENT(_current->_next) && !ROOT(currentHierNode->_parent)) {
			// last of non-root parent, jump to next
			_current = currentHierNode->_parent->_next;
			return *this;
		}
	}

	_current = _current->_next;
	return *this;
}

HierarchyIterator HierarchyIterator::operator++(int) noexcept {
	auto temp = *this;
	++(*this);
	return temp;
}

HierarchyIterator& HierarchyIterator::operator--() noexcept {
	if (!SENT(_current)) {
		if (!SENT(_current->_prev)) {
			auto currentPrevLast = NODE(_current->_prev)->_last;
			if (currentPrevLast != nullptr) {
				// prev has children, jump to _last
				_current = currentPrevLast;
				return *this;
			}
		} else if (auto currentParent = NODE(_current)->_parent; !ROOT(currentParent)) {
			// _last children of non-root parent, jump to parent
			_current = currentParent;
			return *this;
		}
	}

	_current = _current->_prev;
	return *this;
}

HierarchyIterator HierarchyIterator::operator--(int) noexcept {
	auto temp = *this;
	--(*this);
	return temp;
}

} // namespace arch::hier::_details

#undef SENT
#undef ROOT
#undef NODE
