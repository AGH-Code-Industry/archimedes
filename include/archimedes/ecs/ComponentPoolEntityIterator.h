#pragma once

#include "ComponentPoolIterator.h"
#include "Entity.h"

namespace arch::ecs::_details {

template<class C>
class ComponentPoolEntityIterator {
	using Traits = _details::ComponentTraits<C>;

public:
	using Base = ComponentPoolIterator<C>;
	using difference_type = std::ptrdiff_t;
	using value_type = Entity;
	using pointer = const Entity*;
	using reference = const Entity&;
	using iterator_category = Base::IteratorCategory;
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
	const Entity& operator[](std::ptrdiff_t n) const noexcept;

	ComponentPoolEntityIterator& operator+=(std::ptrdiff_t n) noexcept requires(!Traits::inPlace);
	ComponentPoolEntityIterator operator+(std::ptrdiff_t n) const noexcept requires(!Traits::inPlace);

	ComponentPoolEntityIterator& operator-=(std::ptrdiff_t n) noexcept requires(!Traits::inPlace);
	ComponentPoolEntityIterator operator-(std::ptrdiff_t n) const noexcept requires(!Traits::inPlace);

	std::ptrdiff_t operator-(const ComponentPoolEntityIterator& other) const noexcept requires(!Traits::inPlace);

	/// @brief Equality operator
	bool operator==(const ComponentPoolEntityIterator& other) const noexcept;
	/// @brief Comparision operator
	std::strong_ordering operator<=>(const ComponentPoolEntityIterator& other) const noexcept;

private:
	friend arch::ecs::ComponentPool<C>;

	using ETraits = _details::EntityTraits;

	// iterating through pages with offset was tested to be the fastest
	ComponentPoolIterator<C> _base;
};

template<class C>
requires(!_details::ComponentTraits<C>::inPlace)
ComponentPoolEntityIterator<C> operator+(std::ptrdiff_t n, const ComponentPoolEntityIterator<C>& i) noexcept;

} // namespace arch::ecs::_details

#include "ComponentPoolEntityIterator.hpp"
