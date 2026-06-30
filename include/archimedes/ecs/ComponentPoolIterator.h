#pragma once

#include <utility>

#include "ComponentTraits.h"

namespace arch::ecs {

template<class C>
class ComponentPool;

}

namespace arch::ecs::_details {

/// @brief Iterator for ComponentPool
/// @details Models std::bidirectional_iterator
/// @tparam C - component type
template<class C>
class ComponentPoolIterator {
	using Traits = _details::ComponentTraits<C>;

public:

	/// @brief Component type
	using ComponentT = C;
	/// @brief Return value
	// using ValueType = std::pair<const Entity&, std::conditional_t<Traits::flag, const bool, C&>>;
	using ValueType = std::conditional_t<Traits::flag, const bool, C>;
	/// @brief Return value reference
	using Reference = ValueType&;
	/// @brief Return value Pointer
	using Pointer = ValueType*;
	/// @brief Difference type
	using DifferenceType = std::make_signed_t<size_t>;
	/// @brief Iterator category
	using IteratorCategory =
		std::conditional_t<Traits::inPlace, std::bidirectional_iterator_tag, std::random_access_iterator_tag>;
	/// @brief Iterator concept
	using IteratorConcept =
		std::conditional_t<Traits::inPlace, std::bidirectional_iterator_tag, std::random_access_iterator_tag>;

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
	ComponentPoolIterator& operator++() noexcept requires(Traits::inPlace);
	/// @brief Pre-increement operator
	ComponentPoolIterator& operator++() noexcept requires(!Traits::inPlace);
	/// @brief Post-increement operator
	ComponentPoolIterator operator++(int) noexcept;
	/// @brief Pre-decreement operator
	ComponentPoolIterator& operator--() noexcept requires(Traits::inPlace);
	/// @brief Pre-decreement operator
	ComponentPoolIterator& operator--() noexcept requires(!Traits::inPlace);
	/// @brief Post-decreement operator
	ComponentPoolIterator operator--(int) noexcept;

	/// @brief Dereference operator
	Reference operator*() const noexcept;
	/// @brief Access operator
	Pointer operator->() const noexcept;
	Reference operator[](std::ptrdiff_t n) const noexcept;

	ComponentPoolIterator& operator+=(std::ptrdiff_t n) noexcept requires(!Traits::inPlace);
	ComponentPoolIterator operator+(std::ptrdiff_t n) const noexcept requires(!Traits::inPlace);

	ComponentPoolIterator& operator-=(std::ptrdiff_t n) noexcept requires(!Traits::inPlace);
	ComponentPoolIterator operator-(std::ptrdiff_t n) const noexcept requires(!Traits::inPlace);

	std::ptrdiff_t operator-(const ComponentPoolIterator& other) const noexcept requires(!Traits::inPlace);

	const Entity& entity() const noexcept;

	/// @brief Equality operator
	bool operator==(const ComponentPoolIterator& other) const noexcept;
	/// @brief Comparision operator
	std::strong_ordering operator<=>(const ComponentPoolIterator& other) const noexcept;

private:
	friend arch::ecs::ComponentPool<C>;

	using ETraits = _details::EntityTraits;

	// used by ComponentPool
	ComponentPoolIterator(ComponentPool<C>* pool, size_t i = 0) noexcept;

	// iterating through pages with offset was tested to be the fastest
	C** _componentPage;
	size_t _offset;
	std::vector<Entity>* _dense;
	size_t _i;
};

template<class C>
requires(!_details::ComponentTraits<C>::inPlace)
ComponentPoolIterator<C> operator+(std::ptrdiff_t n, const ComponentPoolIterator<C>& i) noexcept;

} // namespace arch::ecs::_details

namespace std {
/// @brief std::iterator_traits' specialization for ComponentPoolIterator
/// @tparam C - component type
/// @tparam E - entity type
template<class C>
struct iterator_traits<arch::ecs::_details::ComponentPoolIterator<C>> {
private:

	using Iter = arch::ecs::_details::ComponentPoolIterator<C>;

public:

	using difference_type = Iter::DifferenceType;
	using value_type = Iter::ValueType;
	using pointer = Iter::Pointer;
	using reference = Iter::Reference;
	using iterator_category = Iter::IteratorCategory;
	using iterator_concept = Iter::IteratorConcept;
};

} // namespace std

#include "ComponentPoolIterator.hpp"
