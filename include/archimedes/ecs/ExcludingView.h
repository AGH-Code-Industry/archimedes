#pragma once

#include "ViewFwd.h"
//
#include "ComponentPool.h"
#include "ExcludeT.h"
#include "ExcludingViewIterator.h"

namespace arch::ecs {

class Domain;

template<class... Excludes>
class View<TypeList<>, TypeList<Excludes...>>:
	public std::ranges::view_interface<View<TypeList<>, TypeList<Excludes...>>> {
public:
	static constexpr size_t includeCount = 0;
	static constexpr size_t excludeCount = sizeof...(Excludes);

	using Iterator = ViewIterator<TypeList<>, TypeList<Excludes...>>;

	using IncludeTL = TypeList<>;
	using ExcludeTL = TypeList<Excludes...>;

	View(Domain* domain) noexcept;

	Iterator begin() const noexcept;
	Iterator cbegin() const noexcept;
	Iterator end() const noexcept;
	Iterator cend() const noexcept;

	template<class Fn>
	void forEach(Fn&& fn) noexcept;

	auto entities() const noexcept;

	bool contains(const Entity entity) const noexcept;

private:
	template<class, class>
	friend class ViewIterator;

	using ExCPools = std::array<const _details::CommonComponentPool*, excludeCount>;

	const EntityPool* _pool;
	ExCPools _excludedCpools{};
};

} // namespace arch::ecs

#include "ExcludingView.hpp"
