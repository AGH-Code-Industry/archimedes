#pragma once

#include "ViewFwd.h"
//
#include "../CommonComponentPool.h"
#include "../Entity.h"
#include <archimedes/utils/TypeList.h>

namespace arch::ecs {

template<class... Includes, class... Excludes>
requires(sizeof...(Includes) != 0 || sizeof...(Excludes) != 0)
class ViewIterator<TypeList<Includes...>, TypeList<Excludes...>> {
public:
	using iterator_concept = std::bidirectional_iterator_tag;
	using iterator_category = std::bidirectional_iterator_tag;
	using value_type = Entity;
	using pointer = const Entity*;
	using reference = const Entity&;
	using difference_type = std::ptrdiff_t;

	static constexpr auto includes = typelist<Includes...>;
	static constexpr auto excludes = typelist<Excludes...>;

	ViewIterator() noexcept = default;
	ViewIterator(const View<TypeList<Includes...>, TypeList<Excludes...>>& view, bool end) noexcept
		requires(includes.size() != 0);
	ViewIterator(const View<TypeList<Includes...>, TypeList<Excludes...>>& view, bool end) noexcept
		requires(includes.size() == 0);

	bool operator==(const ViewIterator& other) const noexcept;
	std::strong_ordering operator<=>(const ViewIterator& other) const noexcept;

	const Entity& operator*() const noexcept;
	const Entity* operator->() const noexcept;

	ViewIterator& operator++() noexcept requires(includes.size() > 1 && excludes.size() == 0);
	ViewIterator& operator++() noexcept requires(includes.size() == 1 && excludes.size() == 0);
	ViewIterator& operator++() noexcept requires(includes.size() != 0 && excludes.size() != 0);
	ViewIterator& operator++() noexcept requires(includes.size() == 0 && excludes.size() != 0);

	ViewIterator operator++(int) noexcept;

	ViewIterator& operator--() noexcept requires(includes.size() > 1 && excludes.size() == 0);
	ViewIterator& operator--() noexcept requires(includes.size() == 1 && excludes.size() == 0);
	ViewIterator& operator--() noexcept requires(includes.size() != 0 && excludes.size() != 0);
	ViewIterator& operator--() noexcept requires(includes.size() == 0 && excludes.size() != 0);

	ViewIterator operator--(int) noexcept;

	// private:

	using Traits = ecs::_details::EntityTraits::Version;

	std::array<_details::CommonComponentPool*, includes.size()>::const_iterator _begin{}, _min{}, _end{};
	std::array<const _details::CommonComponentPool*, excludes.size()>::const_iterator _exclBegin{}, _exclEnd{};

	const Entity *_denseBegin{}, *_denseI{}, *_denseEnd{};
};

} // namespace arch::ecs

#include "GeneralViewIterator.hpp"
