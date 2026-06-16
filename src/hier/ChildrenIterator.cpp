#include <archimedes/Hier.h>

namespace arch::hier {

ChildrenIterator::ChildrenIterator(const ChildNode* node) noexcept: _current{ node } {}

ecs::Entity ChildrenIterator::operator*() const noexcept {
	return _current->_entity;
}

ChildrenIterator& ChildrenIterator::operator++() noexcept {
	_current = _current->_next;
	return *this;
}

ChildrenIterator ChildrenIterator::operator++(int) noexcept {
	auto temp = *this;
	++(*this);
	return temp;
}

ChildrenIterator& ChildrenIterator::operator--() noexcept {
	_current = _current->_prev;
	return *this;
}

ChildrenIterator ChildrenIterator::operator--(int) noexcept {
	auto temp = *this;
	--(*this);
	return temp;
}

} // namespace arch::hier
