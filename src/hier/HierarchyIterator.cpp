#include <hier/HierarchyNode.h>

// is sentinel?
#define SENT(x) ((x)->entity == ::arch::ecs::nullEntity)
// is root?
#define ROOT(x) ((x)->parent == nullptr)
// cast ChildNode* x to HierarchyNode*, used only if !SENT(x)
#define NODE(x) (static_cast<const ::arch::hier::HierarchyNode*>((x)))

namespace arch::hier {

HierarchyIterator::HierarchyIterator(const ChildNode* node) noexcept: _current{ node } {}

ecs::Entity HierarchyIterator::operator*() const noexcept {
	ARCH_ASSERT(!SENT(_current) && !ROOT(NODE(_current)), "HierarchyIterator cannot dereference sentinels or roots");
	return _current->entity;
}

HierarchyIterator& HierarchyIterator::operator++() noexcept {
	if (!SENT(_current)) {
		auto currentHierNode = NODE(_current);
		if (currentHierNode->first != nullptr) {
			_current = currentHierNode->first;
		} else if (!SENT(_current->next) || ROOT(currentHierNode->parent)) {
			_current = _current->next;
		} else {
			_current = currentHierNode->parent->next;
		}
	} else {
		_current = _current->next;
	}
	return *this;
}

HierarchyIterator HierarchyIterator::operator++(int) noexcept {
	auto temp = *this;
	++(*this);
	return temp;
}

HierarchyIterator& HierarchyIterator::operator--() noexcept {
	if (SENT(_current) || !SENT(_current->prev)) {
		auto prevSibling = NODE(_current->prev);
		_current = prevSibling->count != 0 ? prevSibling->last : prevSibling;
	} else if (auto currentHierNode = NODE(_current); !ROOT(currentHierNode->parent)) {
		_current = currentHierNode->parent;
	} else {
		_current = _current->prev;
	}

	return *this;
}

HierarchyIterator HierarchyIterator::operator--(int) noexcept {
	auto temp = *this;
	--(*this);
	return temp;
}

} // namespace arch::hier

#undef SENT
#undef ROOT
#undef NODE
