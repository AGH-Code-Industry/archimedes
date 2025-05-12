#pragma once

#include "ViewFwd.h"
//
#include "ComponentPool.h"
#include "ExcludeT.h"
#include "ViewIterator.h"

namespace arch::ecs {

class Domain;

/// @brief View of components
/// @tparam ...Includes - Component types to include (supports const)
/// @tparam ...Excludes - Component types to exclude
template<class... Includes, class... Excludes>
class View<TypeList<Includes...>, TypeList<Excludes...>>:
	public std::ranges::view_interface<View<TypeList<Includes...>, TypeList<Excludes...>>> {
public:

	/// @brief Count of included components
	static constexpr size_t includeCount = sizeof...(Includes);
	/// @brief Count of excluded components
	static constexpr size_t excludeCount = sizeof...(Excludes);

	static_assert(includeCount != 0, "Cannot have view of no components");

	/// @brief Iterator type
	using Iterator = ViewIterator<TypeList<Includes...>, TypeList<Excludes...>>;

	/// @brief TypeList of Includes
	using IncludeTL = TypeList<Includes...>;

	/// @brief Domain constructor
	/// @param domain - domain pointer to make View of
	View(Domain* domain) noexcept;

	/// @brief Iterator to first entity
	Iterator begin() const noexcept;
	/// @brief Iterator to first entity
	Iterator cbegin() const noexcept;
	/// @brief Iterator to past-the-last entity
	Iterator end() const noexcept;
	/// @brief Iterator to past-the-last entity
	Iterator cend() const noexcept;

	/// @brief Calls fn, for each wanted element
	/// @brief fn can have one of 3 argument lists:
	/// @brief 1. Non-flag components subset, potentially out of order
	/// @brief 2. Entity + 1.
	/// @brief 3. templated args of 1. or 2., in order
	/// @tparam Fn - callable type
	/// @param fn - callable to call
	template<class Fn>
	void forEach(Fn&& fn) noexcept;

	/// @brief Returns iterable of all non-flag components in order
	/// @brief Elements are tuples of references to components
	auto components() noexcept;
	/// @brief Returns iterable of entity with all non-flag components in order
	/// @brief Elements are tuples of Entity and references to components
	auto all() noexcept;

	/// @brief Returns tuple with components of given entity
	auto get(const Entity entity) noexcept;
	/// @brief Checks if given entity is within this View
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

	// returns idx of minimal cpool, or (u32)-1 if at least 1 cpool is invalid.
	u32 _getMinIdx() const noexcept;

	// included cpools
	using CPools = std::array<_details::CommonComponentPool*, includeCount>;
	// excluded cpools
	using ExCPools = std::array<const _details::CommonComponentPool*, excludeCount>;

	CPools _cpools{};
	ExCPools _excludedCpools{};
	u32 _minIdx{};
};

} // namespace arch::ecs

static_assert(std::ranges::view<arch::ecs::View<arch::TypeList<int, float, double>, arch::TypeList<>>>);

#include "View.hpp"
