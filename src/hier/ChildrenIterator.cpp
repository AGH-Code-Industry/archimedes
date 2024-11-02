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
	/*if (_current != ecs::nullEntity) {
		_before = _current;
		auto siblings = _domain->tryGetComponent<Siblings>(_current);
		if (siblings) {
			_current = siblings->get().next;
		}
	} else if (_before != ecs::nullEntity) {
		std::swap(_before, _current);
	}*/
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
	/*if (_current != ecs::nullEntity) {
		_before = _current;
		auto siblings = _domain->tryGetComponent<Siblings>(_current);
		if (siblings) {
			_current = siblings->get().prev;
		}
	} else if (_before != ecs::nullEntity) {
		std::swap(_before, _current);
	}*/
	return *this;
}

ChildrenIterator ChildrenIterator::operator--(int) noexcept {
	auto temp = *this;
	--(*this);
	return temp;
}

} // namespace arch::hier
