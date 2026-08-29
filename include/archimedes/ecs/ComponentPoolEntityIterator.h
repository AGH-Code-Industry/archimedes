#pragma once

#include "ComponentPoolIterator.h"
#include "Entity.h"

namespace arch::ecs::_details {

/// @brief Wrapper for ComponentPoolIterator, returning entities
/// @tparam C - component type
template<class C>
class ComponentPoolEntityIterator {
	using Traits = _details::ComponentTraits<C>;

public:
	/// @brief Base type
	using Base = ComponentPoolIterator<C>;
	/// @brief Difference type
	using difference_type = std::ptrdiff_t;
	/// @brief Value type
	using value_type = Entity;
	/// @brief Pointer type
	using pointer = const Entity*;
	/// @brief Reference type
	using reference = const Entity&;
	/// @brief Base's iterator category
	using iterator_category = Base::IteratorCategory;
	/// @brief Base's iterator concept
	using iterator_concept = Base::IteratorConcept;

	/// @brief Default constructor
	ComponentPoolEntityIterator() noexcept = default;
	/// @brief Copy constructor
	ComponentPoolEntityIterator(const ComponentPoolEntityIterator&) noexcept = default;
	/// @brief Move constructor
	ComponentPoolEntityIterator(ComponentPoolEntityIterator&&) noexcept = default;
	ComponentPoolEntityIterator(const ComponentPoolIterator<C>& base) noexcept;

	/// @brief Copy-assignment operator
	ComponentPoolEntityIterator& operator=(const ComponentPoolEntityIterator&) noexcept = default;
	/// @brief Move-assignment operator
	ComponentPoolEntityIterator& operator=(ComponentPoolEntityIterator&&) noexcept = default;

	/// @brief Swaps with given iterator
	/// @param other - iterator to swap
	void swap(ComponentPoolEntityIterator& other) noexcept;

	/// @brief Pre-increement operator
	ComponentPoolEntityIterator& operator++() noexcept;
	/// @brief Post-increement operator
	ComponentPoolEntityIterator operator++(int) noexcept;
	/// @brief Pre-decreement operator
	ComponentPoolEntityIterator& operator--() noexcept;
	/// @brief Post-decreement operator
	ComponentPoolEntityIterator operator--(int) noexcept;

	/// @brief Dereference operator
	const Entity& operator*() const noexcept;
	/// @brief Access operator
	const Entity* operator->() const noexcept;
	/// @brief Array operator
	const Entity& operator[](std::ptrdiff_t n) const noexcept;

	/// @brief Addition-assignment operator
	ComponentPoolEntityIterator& operator+=(std::ptrdiff_t n) noexcept requires(!Traits::inPlace);
	/// @brief Addition operator
	ComponentPoolEntityIterator operator+(std::ptrdiff_t n) const noexcept requires(!Traits::inPlace);

	/// @brief Subtraction-assignment operator
	ComponentPoolEntityIterator& operator-=(std::ptrdiff_t n) noexcept requires(!Traits::inPlace);
	/// @brief Subtraction operator
	ComponentPoolEntityIterator operator-(std::ptrdiff_t n) const noexcept requires(!Traits::inPlace);

	/// @brief Subtraction operator
	std::ptrdiff_t operator-(const ComponentPoolEntityIterator& other) const noexcept requires(!Traits::inPlace);

	/// @brief Equality operator
	bool operator==(const ComponentPoolEntityIterator& other) const noexcept;
	/// @brief Comparision operator
	std::strong_ordering operator<=>(const ComponentPoolEntityIterator& other) const noexcept;

private:
	friend arch::ecs::ComponentPool<C>;

	using ETraits = _details::EntityTraits;

	ComponentPoolIterator<C> _base;
};

/// @brief Addition operator
template<class C>
requires(!_details::ComponentTraits<C>::inPlace)
ComponentPoolEntityIterator<C> operator+(std::ptrdiff_t n, const ComponentPoolEntityIterator<C>& i) noexcept;

} // namespace arch::ecs::_details

#include "ComponentPoolEntityIterator.hpp"
