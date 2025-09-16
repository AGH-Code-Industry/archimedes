#pragma once

#include <compare>
#include <iterator>
#include <new>

#include "CommonComponentPool.h"
#include "Entity.h"
#include "ViewFwd.h"
#include <tUtils/TypeList.h>

namespace arch::ecs {

/// @brief Iterator of View
/// @tparam ...Includes - component types to include
/// @tparam ...Excludes - component types to exclude
template<class... Includes, class... Excludes>
class ViewIterator<TypeList<Includes...>, TypeList<Excludes...>> {
public:

	/// @brief Count of included components
	static constexpr size_t includeCount = sizeof...(Includes);
	/// @brief Count of excluded components
	static constexpr size_t excludeCount = sizeof...(Excludes);

	/// @brief Default constructor
	ViewIterator() noexcept = default;
	/// @brief View constructor
	/// @param view - pointer to View (usually this)
	/// @param i - offset of _dense
	/// @param valid - whether View is valid
	ViewIterator(
		const View<TypeList<Includes...>, TypeList<Excludes...>>* view,
		const size_t i,
		const bool valid
	) noexcept;

	/// @brief Copy constructor
	ViewIterator(const ViewIterator&) noexcept = default;
	/// @brief Move constructor
	ViewIterator(ViewIterator&&) noexcept = default;

	/// @brief Copy-assignment operator
	ViewIterator& operator=(const ViewIterator&) noexcept = default;
	/// @brief Move-assignment operator
	ViewIterator& operator=(ViewIterator&&) noexcept = default;

	/// @brief Pre-increment operator
	ViewIterator& operator++() noexcept;
	/// @brief Post-increment operator
	ViewIterator operator++(int) noexcept;
	/// @brief Pre-decrement operator
	ViewIterator& operator--() noexcept;
	/// @brief Post-decrement operator
	ViewIterator operator--(int) noexcept;

	/// @brief Dereference operator
	Entity operator*() const noexcept;
	/// @brief Access operator
	const Entity* operator->() const noexcept;

	/// @brief Equality operator
	bool operator==(const ViewIterator& other) const noexcept;
	/// @brief Comparision operator
	auto operator<=>(const ViewIterator& other) const noexcept;

private:

	// cpool ptrs
	using CArrIt = std::array<_details::CommonComponentPool*, includeCount>::const_iterator;
	// excluded cpool ptrs
	using ExArrIt = std::array<const _details::CommonComponentPool*, excludeCount>::const_iterator;

	// iters to included cpools (middle - _cpools[_minIdx])
	CArrIt _begin{}, _middle{}, _end{};

	// iters to _dense
	const Entity* _denseBegin{};
	const Entity* _denseI{};
	const Entity* _denseEnd{};

	// iters to excluded cpools
	ExArrIt _exBegin{}, _exEnd{};
};

} // namespace arch::ecs

/// @brief Iterator traits for ViewIterator
/// @tparam I - TypeList<Includes...>
/// @tparam E - TypeList<Excludes...>
template<class I, class E>
struct std::iterator_traits<arch::ecs::ViewIterator<I, E>> {
	using iterator_category = std::bidirectional_iterator_tag;
	using value_type = arch::ecs::Entity;
	using difference_type = ptrdiff_t;
	using pointer = const value_type*;
	using reference = value_type;
};

// ViewIterator models bidirectional_iterator
static_assert(std::bidirectional_iterator<
			  arch::ecs::ViewIterator<arch::TypeList<int, float, double>, arch::TypeList<>>>);

#include "ViewIterator.hpp"
