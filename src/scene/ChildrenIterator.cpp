#include <archimedes/scene/ChildrenIterator.h>
#include <archimedes/scene/Entity.h>

namespace arch::scene {

ChildrenIterator::ChildrenIterator(hier::ChildrenIterator base, Entity* parent) noexcept:
	_current{ std::move(base) },
	_parent{ parent } {}

Entity ChildrenIterator::operator*() const noexcept {
	return Entity(*_parent->_scene, *_current);
}

ChildrenIterator& ChildrenIterator::operator++() noexcept {
	++_current;
	return *this;
}

ChildrenIterator ChildrenIterator::operator++(int) noexcept {
	auto temp = *this;
	++(*this);
	return temp;
}

ChildrenIterator& ChildrenIterator::operator--() noexcept {
	--_current;
	return *this;
}

ChildrenIterator ChildrenIterator::operator--(int) noexcept {
	auto temp = *this;
	--(*this);
	return temp;
}

} // namespace arch::scene
