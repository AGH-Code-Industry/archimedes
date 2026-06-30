#pragma once

#include "ViewFwd.h"
//
#include "../CommonComponentPool.h"
#include "../Entity.h"
#include <archimedes/utils/TypeList.h>

namespace arch::ecs {

/// @brief Iterator of a Single-include view, where component is not in-place
template<class Include>
requires(!_details::ComponentTraits<Include>::inPlace)
class ViewIterator<TypeList<Include>, TypeList<>> {
public:
	/// @brief Makes iterator random access
	using iterator_concept = std::random_access_iterator_tag;
	/// @brief Makes iterator random access
	using iterator_category = std::random_access_iterator_tag;
	/// @brief Iterator's value type
	using value_type = Entity;
	/// @brief Iterator's pointer type
	using pointer = const Entity*;
	/// @brief Iterator's reference type
	using reference = const Entity&;
	/// @brief Iterator's difference type
	using difference_type = std::ptrdiff_t;

	/// @brief Typelist with included types
	static constexpr auto includes = typelist<Include>;
	/// @brief Typelist with excluded types
	static constexpr auto excludes = typelist<>;

	/// @brief Default constructor
	ViewIterator() noexcept = default;
	/// @brief Copy constructor
	ViewIterator(const ViewIterator&) noexcept = default;
	/// @brief Move constructor
	ViewIterator(ViewIterator&&) noexcept = default;

	/// @brief Copy-assignment operator
	ViewIterator& operator=(const ViewIterator&) noexcept = default;
	/// @brief Move-assignment operator
	ViewIterator& operator=(ViewIterator&&) noexcept = default;

	/// @brief View-based constructor
	ViewIterator(const View<TypeList<Include>, TypeList<>>& view, bool end) noexcept;

	/// @brief Comparision operator
	bool operator==(const ViewIterator& other) const noexcept;
	/// @brief Comparision operator
	std::strong_ordering operator<=>(const ViewIterator& other) const noexcept;

	/// @brief Dereference operator
	const Entity& operator*() const noexcept;
	/// @brief Access operator
	const Entity* operator->() const noexcept;
	/// @brief Array operator
	const Entity& operator[](std::ptrdiff_t n) const noexcept;

	/// @brief Preincrement operator
	ViewIterator& operator++() noexcept;
	/// @brief Postincrement operator
	ViewIterator operator++(int) noexcept;

	/// @brief Predecrement operator
	ViewIterator& operator--() noexcept;
	/// @brief Postdecrement operator
	ViewIterator operator--(int) noexcept;

	/// @brief Addition-assignment operator
	ViewIterator& operator+=(std::ptrdiff_t n) noexcept;
	/// @brief Addition operator
	ViewIterator operator+(std::ptrdiff_t n) const noexcept;

	/// @brief Subtraction-assignment operator
	ViewIterator& operator-=(std::ptrdiff_t n) noexcept;
	/// @brief Subtraction operator
	ViewIterator operator-(std::ptrdiff_t n) const noexcept;

	/// @brief Difference operator
	std::ptrdiff_t operator-(const ViewIterator& other) const noexcept;

private:

	const Entity* _denseI{};
};

/// @brief Addition operator
template<class Include>
requires(!_details::ComponentTraits<Include>::inPlace)
ViewIterator<TypeList<Include>, TypeList<>> operator+(
	std::ptrdiff_t n,
	const ViewIterator<TypeList<Include>, TypeList<>>& i
) noexcept;

} // namespace arch::ecs

#include "SingleContignousViewIterator.hpp"
