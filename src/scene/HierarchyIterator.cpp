#include <scene/HierarchyIterator.h>
//
#include <scene/Entity.h>

namespace arch::scene {

HierarchyIterator::HierarchyIterator(hier::_details::HierarchyIterator base, Entity* parent) noexcept:
	_current{ std::move(base) },
	_parent{ parent } {}

Entity HierarchyIterator::operator*() const noexcept {
	return Entity(*_parent->_scene, *_current);
}

HierarchyIterator& HierarchyIterator::operator++() noexcept {
	++_current;
	return *this;
}

HierarchyIterator HierarchyIterator::operator++(int) noexcept {
	auto temp = *this;
	++(*this);
	return temp;
}

HierarchyIterator& HierarchyIterator::operator--() noexcept {
	--_current;
	return *this;
}

HierarchyIterator HierarchyIterator::operator--(int) noexcept {
	auto temp = *this;
	--(*this);
	return temp;
}

} // namespace arch::scene
