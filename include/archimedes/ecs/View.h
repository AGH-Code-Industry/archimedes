#pragma once

#include <ranges>
#include <type_traits>

#include "ComponentPool.h"
#include "EntityPool.h"
#include "ExcludeT.h"

namespace arch::ecs {

class Domain;

namespace _details {

/// @brief Returns given component wrapped in std::tuple
/// @tparam Entity - entity type
/// @tparam C - component type
/// @param domain - domain to get component from
/// @param entity - entity to get component from
template<class C>
auto getAsTuple(Domain& domain, const Entity entity) noexcept;
/// @brief Returns given readonly component wrapped in std::tuple
/// @tparam Entity - entity type
/// @tparam C - component type
/// @param domain - domain to get component from
/// @param entity - entity to get component from
template<class C>
auto getAsTuple(const Domain& domain, const Entity entity) noexcept;
/// @brief Returns std::tuple with specified components from given entity
/// @tparam Entity - entity type
/// @tparam Includes - component types
/// @param domain - domain to get components from
/// @param entity - entity to get components from
/// @param <unnamed> - TypeList instance to help in deduction
template<class... Includes>
auto getByTL(Domain& domain, const Entity entity, TypeList<Includes...>) noexcept;
/// @brief Returns std::tuple with specified components from given entity
/// @tparam Entity - entity type
/// @tparam Includes - component types
/// @param domain - domain to get components from
/// @param entity - entity to get components from
/// @param <unnamed> - TypeList instance to help in deduction
template<class... Includes>
auto getByTL(const Domain& domain, const Entity entity, TypeList<Includes...>) noexcept;

/// @brief Predicate for TypeList to filter out flag-components
/// @tparam C - component type
template<class C>
struct IsFlagComponent {
	static inline constexpr bool value = _details::ComponentTraits<C>::flag;
};
} // namespace _details

/// @brief Domain view
/// @tparam Entity - entity type
/// @tparam Const - whether view is readonly
/// @tparam Include - TypeList with components included in view
/// @tparam Exclude - TypeList with components excluded in view
template<bool Const, class Include, class Exclude>
class View;

/// @brief Domain's view
/// @details Models std::ranges::view
/// @tparam Entity - entity type
/// @tparam Const - whether view is readonly
/// @tparam Includes - components included in view
/// @tparam Excludes - components excluded in view
template<bool Const, class... Includes, class... Excludes>
class View<Const, TypeList<Includes...>, TypeList<Excludes...>>:
	public std::ranges::view_interface<View<Const, TypeList<Includes...>, TypeList<Excludes...>>> {
public:

	/// @brief Entity traits
	using Traits = _details::EntityTraits;
	/// @brief Actual domain type used
	using DomainT = std::conditional_t<Const, const Domain, Domain>;

	/// @brief TypeList with components included in view
	using Include = TypeList<Includes...>;
	/// @brief TypeList with components excluded in view
	using Exclude = TypeList<Excludes...>;
	/// @brief Include without flag-components
	using NoFlags = typename Include::template eraseIf<_details::IsFlagComponent>;

	/// @brief Count of included component types
	static inline constexpr size_t includeCount = Include::length;
	/// @brief Count of excluded component types
	static inline constexpr size_t excludeCount = Exclude::length;
	/// @brief Whether view is readonly
	static inline constexpr bool readonly = Const;

	View(const View& other) noexcept;
	View(View&& other) noexcept;

	View& operator=(const View&) noexcept;
	View& operator=(View&& other) noexcept;

	/// @brief Checks if entity is contained by view
	/// @param entity - entity to check
	bool contains(const Entity entity) const noexcept;

	/// @brief Recomputes entities in view
	/// @brief Needed if at least one component is in-place
	View& refresh() noexcept;

	/// @brief Returns std::tuple with included components, excluding flag-components
	/// @param entity - entity to get components from
	auto get(const Entity entity) noexcept requires(!Const);
	/// @brief Returns std::tuple with readonly included components, excluding flag-components
	/// @param entity - entity to get components from
	auto get(const Entity entity) const noexcept;
	/// @brief Returns std::tuple with included components, including flag-components
	/// @param entity - entity to get components from
	auto getAll(const Entity entity) noexcept requires(!Const);
	/// @brief Returns std::tuple with readonly included components, including flag-components
	/// @param entity - entity to get components from
	auto getAll(const Entity entity) const noexcept;
	/// @brief Returns std::tuple with given components
	/// @tparam Cs - components to get
	/// @param entity - entity to get components from
	template<class... Cs>
	auto get(const Entity entity) noexcept requires(!Const);
	/// @brief Returns std::tuple with given readonly components
	/// @tparam Cs - components to get
	/// @param entity - entity to get components from
	template<class... Cs>
	auto get(const Entity entity) const noexcept;

	/// @brief Returns std::view of std::tuples
	/// @brief First element is the entity
	/// @brief Rest are non-flag-components
	auto all() noexcept requires(!Const);
	/// @brief Returns readonly std::view of std::tuples
	/// @brief First element is the entity
	/// @brief Rest are non-flag-components
	auto all() const noexcept;
	/// @brief Returns std::view of std::tuples
	/// @brief Elements are components (flag-components too)
	auto components() noexcept requires(!Const);
	/// @brief Returns readonly std::view of std::tuples
	/// @brief Elements are components (flag-components too)
	auto components() const noexcept;
	/// @brief Returns std::view of std::tuples
	/// @brief Elements are components (flag-components too)
	/// @tparam Cs - components to include, must be a subset of Include
	template<class... Cs>
	auto components() noexcept requires(!Const);
	/// @brief Returns readonly std::view of std::tuples
	/// @brief Elements are components (flag-components too)
	/// @tparam Cs - components to include, must be a subset of Include
	template<class... Cs>
	auto components() const noexcept;

	/// @brief Executes given function in one of 5 modes (deduced from function's signature), with elements of view as
	/// arguments
	/// @details Mode 1: entites
	/// @details Mode 2: entites & non-flag-components
	/// @details Mode 3: entites & all components
	/// @details Mode 4: non-flag-components
	/// @details Mode 5: all components
	/// @tparam Fn - function type
	/// @param fn - function object
	template<class Fn>
	void forEach(Fn&& fn) noexcept requires(!Const);
	/// @brief Executes given function in one of 5 modes (deduced from function's signature), with readonly elements of
	/// view as arguments
	/// @details Mode 1: entites
	/// @details Mode 2: entites & non-flag-components
	/// @details Mode 3: entites & all components
	/// @details Mode 4: non-flag-components
	/// @details Mode 5: all components
	/// @tparam Fn - function type
	/// @param fn - function object
	template<class Fn>
	void forEach(Fn&& fn) const noexcept;

	/// @brief Returns iterator to beginning of view
	auto begin() const noexcept;
	/// @brief Returns iterator to beginning of view
	auto cbegin() const noexcept;
	/// @brief Returns iterator to end of view
	auto end() const noexcept;
	/// @brief Returns iterator to end of view
	auto cend() const noexcept;

private:

	friend Domain;

	using CCPoolPtr = const _details::CommonComponentPool*;
	//, _details::CommonComponentPool<Entity>* > ;
	template<class C>
	using CPoolPtr = std::conditional_t<
		Const || std::is_const_v<C>,
		const ComponentPool<std::remove_const_t<C>>*,
		ComponentPool<std::remove_const_t<C>>*>;

	size_t _minInclude() const noexcept;

	template<class C>
	auto getAsTuple(const Entity entity) noexcept requires(!Const);
	template<class C>
	auto getAsTuple(const Entity entity) const noexcept;
	template<class... Cs>
	auto getByTL(const Entity entity, TypeList<Cs...>) noexcept requires(!Const);
	template<class... Cs>
	auto getByTL(const Entity entity, TypeList<Cs...>) const noexcept;

	// expected type of entities view
	using EntitesViewT = decltype(std::views::filter(
		std::declval<const _details::CommonComponentPool>()._entitiesForView(),
		std::bind(&View::contains, std::declval<const View*>(), std::placeholders::_1)
	));

	View(DomainT* domain) noexcept;

	std::array<CCPoolPtr, includeCount> _includedCPools;
	std::array<CCPoolPtr, excludeCount> _excludedCPools;
	size_t _minIdx = (size_t)-1;
	mutable EntitesViewT _entities;
};

/// @brief Domain's exclude-only view
/// @tparam Entity - entity type
/// @tparam Const - whether view is readonly
/// @tparam Excludes - components excluded in view
template<bool Const, class... Excludes>
class View<Const, TypeList<>, TypeList<Excludes...>>:
	public std::ranges::view_interface<View<Const, TypeList<>, TypeList<Excludes...>>> {
public:

	/// @brief Entity traits
	using Traits = _details::EntityTraits;

	/// @brief TypeList with components included in view
	using Include = TypeList<>;
	/// @brief TypeList with components excluded in view
	using Exclude = TypeList<Excludes...>;
	/// @brief Actual domain type used
	using DomainT = std::conditional_t<Const, const Domain, Domain>;

	/// @brief Count of included component types
	static inline constexpr size_t includeCount = TypeList<>::length;
	/// @brief Count of excluded component types
	static inline constexpr size_t excludeCount = TypeList<Excludes...>::length;
	/// @brief Whether view is readonly
	static inline constexpr bool readonly = Const;

	View(const View& other) noexcept;
	View(View&& other) noexcept;

	View& operator=(const View& other) noexcept;
	View& operator=(View&& other) noexcept;

	/// @brief Checks if entity is contained by view
	/// @param entity - entity to check
	bool contains(const Entity entity) const noexcept;

	/// @brief Recomputes entities in view
	/// @brief Needed if at least one component is in-place
	View& refresh() noexcept;

	/// @brief Returns std::tuple with given components (may be out of view)
	/// @tparam Cs - components to get
	/// @param entity - entity to get components from
	template<class... Cs>
	auto get(const Entity entity) noexcept requires(!Const);
	/// @brief Returns std::tuple with given readonly components (may be out of view)
	/// @tparam Cs - components to get
	/// @param entity - entity to get components from
	template<class... Cs>
	auto get(const Entity entity) const noexcept;

	/// @brief Executes given function with entities in view as arguments
	/// @tparam Fn - function type
	/// @param fn - function object
	template<class Fn>
	void forEach(Fn&& fn) noexcept requires(!Const);
	/// @brief Executes given function with entities in view as arguments
	/// @tparam Fn - function type
	/// @param fn - function object
	template<class Fn>
	void forEach(Fn&& fn) const noexcept;

	/// @brief Returns iterator to beginning of view
	auto begin() const noexcept;
	/// @brief Returns iterator to beginning of view
	auto cbegin() const noexcept;
	/// @brief Returns iterator to end of view
	auto end() const noexcept;
	/// @brief Returns iterator to end of view
	auto cend() const noexcept;

private:

	friend Domain;

	// filtering function
	bool _containsNoCheck(const Entity entity) const noexcept;

	using CCPoolPtr = std::conditional_t<Const, const _details::CommonComponentPool*, _details::CommonComponentPool*>;
	template<class C>
	using CPoolPtr = std::conditional_t<
		Const || std::is_const_v<C>,
		const ComponentPool<std::remove_const_t<C>>*,
		ComponentPool<std::remove_const_t<C>>*>;

	// expected type of entities view
	using EntitesViewT = decltype(std::views::filter(
		std::views::all(*std::declval<const ecs::EntityPool*>()),
		std::bind(&View::_containsNoCheck, std::declval<const View*>(), std::placeholders::_1)
	));

	View(DomainT* domain) noexcept;

	std::array<CCPoolPtr, excludeCount> _excludedCPools;
	mutable EntitesViewT _entities;
	DomainT* _domain;
};

} // namespace arch::ecs

#include "View.hpp"
