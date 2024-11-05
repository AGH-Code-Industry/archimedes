#include <Hier.h>

namespace arch::hier {

ChildrenIterator::ChildrenIterator(const ChildNode* node) noexcept: _current{ node } {}

ecs::Entity ChildrenIterator::operator*() const noexcept {
	return _current->entity;
}

ChildrenIterator& ChildrenIterator::operator++() noexcept {
	if (_current->next != nullptr) {
		_current = _current->next;
	}
	return *this;
}

ChildrenIterator ChildrenIterator::operator++(int) noexcept {
	auto temp = *this;
	++(*this);
	return temp;
}

ChildrenIterator& ChildrenIterator::operator--() noexcept {
	if (_current->prev != nullptr) {
		_current = _current->prev;
	}
	return *this;
}

ChildrenIterator ChildrenIterator::operator--(int) noexcept {
	auto temp = *this;
	--(*this);
	return temp;
}

} // namespace arch::hier
