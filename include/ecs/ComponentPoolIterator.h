#pragma once

#include <utility>

#include "ComponentTraits.h"

namespace arch::ecs {

template<class C, class E>
class ComponentPool;

}

namespace arch::ecs::_details {

/// @brief Iterator for ComponentPool
/// @details Models std::bidirectional_iterator
/// @tparam C - component type
/// @tparam E - entity type
template<class C, class E>
class ComponentPoolIterator {
	using Traits = _details::ComponentTraits<C, E>;

public:

	/// @brief Component type
	using ComponentT = C;
	/// @brief Entity type
	using EntityT = E;
	/// @brief Return value
	using ValueType = std::pair<const E&, std::conditional_t<Traits::flag, const bool, C&>>;
	/// @brief Return value reference
	using Reference = ValueType&;
	/// @brief Return value Pointer
	using Pointer = ValueType*;
	/// @brief Difference type
	using DifferenceType = std::make_signed_t<size_t>;
	/// @brief Iterator category
	using IteratorCategory = std::bidirectional_iterator_tag;

	/// @brief Default constructor
	ComponentPoolIterator() noexcept = default;
	/// @brief Copy constructor
	ComponentPoolIterator(const ComponentPoolIterator&) noexcept = default;
	/// @brief Move constructor
	ComponentPoolIterator(ComponentPoolIterator&&) noexcept = default;

	/// @brief Copy-assignment operator
	ComponentPoolIterator& operator=(const ComponentPoolIterator&) noexcept = default;
	/// @brief Move-assignment operator
	ComponentPoolIterator& operator=(ComponentPoolIterator&&) noexcept = default;

	/// @brief Swaps with given iterator
	/// @param other - iterator to swap
	void swap(ComponentPoolIterator& other) noexcept;

	/// @brief Pre-increement operator
	ComponentPoolIterator& operator++() noexcept;
	/// @brief Post-increement operator
	ComponentPoolIterator operator++(int) noexcept;
	/// @brief Pre-decreement operator
	ComponentPoolIterator& operator--() noexcept;
	/// @brief Post-decreement operator
	ComponentPoolIterator operator--(int) noexcept;

	/// @brief Dereference operator
	Reference operator*() const noexcept;
	/// @brief Access operator
	Pointer operator->() const noexcept;

	/// @brief Equality operator
	bool operator==(const ComponentPoolIterator& other) const noexcept;
	/// @brief Comparision operator
	std::strong_ordering operator<=>(const ComponentPoolIterator& other) const noexcept;

private:
	friend arch::ecs::ComponentPool<C, E>;

	// if iterator is valid
	bool _valid() const noexcept;
	void _update() noexcept;

	using ETraits = _details::EntityTraits<E>;

	// used by ComponentPool
	ComponentPoolIterator(ComponentPool<C, E>* pool, size_t i = 0) noexcept;

	Reference _pair() const noexcept;

	// f*ck default-constructibleness
	// Fine... I'll do it myself
	alignas(ValueType) char _value[sizeof(ValueType)]{};

	// iterating through pages with offset was tested to be the fastest
	C** _componentPage;
	size_t _offset;
	std::vector<E>* _dense;
	size_t _i;
};

} // namespace arch::ecs::_details

namespace std {
/// @brief std::iterator_traits' specialization for ComponentPoolIterator
/// @tparam C - component type
/// @tparam E - entity type
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
