#pragma once

#include <compare>
#include <iterator>
#include <new>

#include "Entity.h"
#include "ViewFwd.h"
#include <tUtils/TypeList.h>

namespace arch::ecs {

template<class... Includes, class... Excludes>
class ViewIterator<TypeList<Includes...>, TypeList<Excludes...>> {
public:

	static constexpr size_t includeCount = sizeof...(Includes);

	ViewIterator() noexcept = default;
	ViewIterator(
		const View<TypeList<Includes...>, TypeList<Excludes...>>* view,
		const size_t i,
		const bool valid
	) noexcept;

	ViewIterator(const ViewIterator&) noexcept = default;
	ViewIterator(ViewIterator&&) noexcept = default;

	ViewIterator& operator=(const ViewIterator&) noexcept = default;
	ViewIterator& operator=(ViewIterator&&) noexcept = default;

	ViewIterator& operator++() noexcept;
	ViewIterator operator++(int) noexcept;
	ViewIterator& operator--() noexcept;
	ViewIterator operator--(int) noexcept;

	Entity operator*() const noexcept;
	const Entity* operator->() const noexcept;

	bool operator==(const ViewIterator& other) const noexcept;
	auto operator<=>(const ViewIterator& other) const noexcept;

private:

	using ArrIt = std::array<_details::CommonComponentPool*, sizeof...(Includes)>::const_iterator;
	using ExArrIt = std::array<const _details::CommonComponentPool*, sizeof...(Excludes)>::const_iterator;

	ArrIt _begin{}, _middle{}, _end{};
	const Entity* _denseBegin{};
	const Entity* _denseI{};
	const Entity* _denseEnd{};
	ExArrIt _exBegin{}, _exEnd{};
};

} // namespace arch::ecs

template<class I, class E>
struct std::iterator_traits<arch::ecs::ViewIterator<I, E>> {
	using iterator_category = std::bidirectional_iterator_tag;
	using value_type = arch::ecs::Entity;
	using difference_type = ptrdiff_t;
	using pointer = const value_type*;
	using reference = value_type;
};

static_assert(std::bidirectional_iterator<
			  arch::ecs::ViewIterator<arch::TypeList<int, float, double>, arch::TypeList<>>>);

#include "ViewIterator.hpp"
