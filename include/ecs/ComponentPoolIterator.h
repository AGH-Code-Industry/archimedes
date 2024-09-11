#pragma once

#include <utility>

#include "ComponentTraits.h"

namespace arch::ecs {

template<class C, class E>
class ComponentPool;

}

namespace arch::ecs::_details {

template<class C, class E>
class ComponentPoolIterator {
public:

	using ComponentT = C;
	using EntityT = E;
	using ValueType = std::pair<const E&, C&>;
	using Reference = ValueType&;
	using Pointer = ValueType*;
	using DifferenceType = std::make_signed_t<size_t>;
	using IteratorCategory = std::bidirectional_iterator_tag;

	ComponentPoolIterator() noexcept = default;
	ComponentPoolIterator(const ComponentPoolIterator&) noexcept = default;
	ComponentPoolIterator(ComponentPoolIterator&&) noexcept = default;

	ComponentPoolIterator& operator=(const ComponentPoolIterator&) noexcept = default;
	ComponentPoolIterator& operator=(ComponentPoolIterator&&) noexcept = default;

	void swap(ComponentPoolIterator& other) noexcept;

	ComponentPoolIterator& operator++() noexcept;
	ComponentPoolIterator operator++(int) noexcept;
	ComponentPoolIterator& operator--() noexcept;
	ComponentPoolIterator operator--(int) noexcept;

	Reference operator*() const noexcept;
	Pointer operator->() const noexcept;

	bool operator==(const ComponentPoolIterator& other) const noexcept;
	std::strong_ordering operator<=>(const ComponentPoolIterator& other) const noexcept;

private:
	friend arch::ecs::ComponentPool<C, E>;

	bool _valid() const noexcept;

	using Traits = _details::ComponentTraits<C, E>;
	using ETraits = _details::EntityTraits<E>;

	// ComponentPoolIterator(ComponentPool<C, E>* pool) noexcept;
	ComponentPoolIterator(ComponentPool<C, E>* pool, size_t i = 0) noexcept;

	Reference _pair() const noexcept;

	// fck default-constructibleness
	alignas(ValueType) char _value[sizeof(ValueType)]{};
	C** _componentPage;
	size_t _offset;
	std::vector<E>* _dense;
	size_t _i;
};

} // namespace arch::ecs::_details

namespace std {
template<class C, class E>
struct iterator_traits<arch::ecs::_details::ComponentPoolIterator<C, E>> {
private:

	using Iter = arch::ecs::_details::ComponentPoolIterator<C, E>;

public:

	using difference_type = Iter::DifferenceType;
	using value_type = Iter::ValueType;
	using pointer = Iter::Pointer;
	using reference = Iter::Reference;
	using iterator_category = Iter::IteratorCategory;
};
} // namespace std

static_assert(std::bidirectional_iterator<arch::ecs::_details::ComponentPoolIterator<int, arch::ecs::e64>>);

#include "ComponentPoolIterator.hpp"
