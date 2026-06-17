#pragma once

#include "ViewFwd.h"
//
#include "../CommonComponentPool.h"
#include "../Entity.h"
#include <archimedes/utils/TypeList.h>

namespace arch::ecs {

template<class Include>
requires(!_details::ComponentTraits<Include>::inPlace)
class ViewIterator<TypeList<Include>, TypeList<>> {
public:
	using iterator_concept = std::bidirectional_iterator_tag;
	using iterator_category = std::bidirectional_iterator_tag;
	using value_type = Entity;
	using pointer = const Entity*;
	using reference = const Entity&;
	using difference_type = std::ptrdiff_t;

	static constexpr auto includes = typelist<Include>;
	static constexpr auto excludes = typelist<>;

	ViewIterator() noexcept = default;
	ViewIterator(const View<TypeList<Include>, TypeList<>>& view, bool end) noexcept;

	bool operator==(const ViewIterator& other) const noexcept;
	std::strong_ordering operator<=>(const ViewIterator& other) const noexcept;

	const Entity& operator*() const noexcept;
	const Entity* operator->() const noexcept;
	const Entity& operator[](std::ptrdiff_t n) const noexcept;

	ViewIterator& operator++() noexcept;
	ViewIterator operator++(int) noexcept;

	ViewIterator& operator--() noexcept;
	ViewIterator operator--(int) noexcept;

	ViewIterator& operator+=(std::ptrdiff_t n) noexcept;
	ViewIterator operator+(std::ptrdiff_t n) const noexcept;

	ViewIterator& operator-=(std::ptrdiff_t n) noexcept;
	ViewIterator operator-(std::ptrdiff_t n) const noexcept;

	std::ptrdiff_t operator-(const ViewIterator& other) const noexcept;

private:

	const Entity* _denseI{};
};

} // namespace arch::ecs

#include "SingleContignousViewIterator.hpp"
