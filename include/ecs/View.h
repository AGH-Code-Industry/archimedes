#pragma once

#include "ViewFwd.h"
//
#include "ComponentPool.h"
#include "ExcludeT.h"
#include "ViewIterator.h"

namespace arch::ecs {

class Domain;

template<class... Includes, class... Excludes>
class View<TypeList<Includes...>, TypeList<Excludes...>>:
	public std::ranges::view_interface<View<TypeList<Includes...>, TypeList<Excludes...>>> {
public:

	static constexpr size_t includeCount = sizeof...(Includes);
	static constexpr size_t excludeCount = sizeof...(Excludes);

	static_assert(includeCount != 0, "Cannot have view of no components");

	using Iterator = ViewIterator<TypeList<Includes...>, TypeList<Excludes...>>;

	using IncludeTL = TypeList<Includes...>;

	View(Domain* domain) noexcept;

	Iterator begin() const noexcept;
	Iterator cbegin() const noexcept;
	Iterator end() const noexcept;
	Iterator cend() const noexcept;

	template<class Fn>
	void forEach(Fn&& fn) noexcept;

	auto all() noexcept;
	auto components() noexcept;

	auto get(const Entity entity) noexcept;
	bool contains(const Entity entity) const noexcept;

private:

	template<class, class>
	friend class ViewIterator;

	// returns cpool pointer of given component
	// based on constness of T
	template<class T>
	struct SelectCPool {
		using type =
			std::conditional_t<std::is_const_v<T>, const ComponentPool<std::remove_const_t<T>>*, ComponentPool<T>*>;
	};

	// non flag components
	using GetTL = IncludeTL::template eraseIf<_details::FlagComponentPred>;

	// passing TypeList, allows for folding of Cs...
	template<class... Cs>
	auto _all(TypeList<Cs...>) noexcept;
	template<class... Cs>
	auto _components(TypeList<Cs...>) noexcept;
	// passing TypeList, allows for folding of Cs...
	// also allows alternate order of args
	template<bool PassEntity, class Fn, class... Cs>
	void _forEach(Fn&& fn, TypeList<Cs...>) noexcept;
	template<class... Cs>
	auto _get(const Entity entity, TypeList<Cs...>) noexcept;

	u32 _getMinIdx() const noexcept;

	// included cpools
	using CPools = std::array<_details::CommonComponentPool*, includeCount>;
	// excluded cpools
	using ExCPools = std::array<const _details::CommonComponentPool*, excludeCount>;

	CPools _cpools{};
	ExCPools _exCpools{};
	u32 _minIdx{};
};

} // namespace arch::ecs

#include "View.hpp"
