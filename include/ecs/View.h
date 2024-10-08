#pragma once

#include <ranges>
#include <type_traits>

#include "ComponentPool.h"
// #include "ContainChecks.h"
#include "ExcludeT.h"

namespace arch::ecs {

template<class E>
class Domain;

namespace _details {

/// @brief Returns given component wrapped in std::tuple
/// @tparam E - entity type
/// @tparam C - component type
/// @param domain - domain to get component from
/// @param entity - entity to get component from
template<class E, class C>
auto getAsTuple(Domain<E>& domain, const E entity) noexcept;
/// @brief Returns given readonly component wrapped in std::tuple
/// @tparam E - entity type
/// @tparam C - component type
/// @param domain - domain to get component from
/// @param entity - entity to get component from
template<class E, class C>
auto getAsTuple(const Domain<E>& domain, const E entity) noexcept;
/// @brief Returns std::tuple with specified components from given entity
/// @tparam E - entity type
/// @tparam Includes - component types
/// @param domain - domain to get components from
/// @param entity - entity to get components from
/// @param <unnamed> - TypeList instance to help in deduction
template<class E, class... Includes>
auto getByTL(Domain<E>& domain, const E entity, TypeList<Includes...>) noexcept;
/// @brief Returns std::tuple with specified components from given entity
/// @tparam E - entity type
/// @tparam Includes - component types
/// @param domain - domain to get components from
/// @param entity - entity to get components from
/// @param <unnamed> - TypeList instance to help in deduction
template<class E, class... Includes>
auto getByTL(const Domain<E>& domain, const E entity, TypeList<Includes...>) noexcept;

/// @brief Predicate wrapper for TypeList to filter out flag-components
/// @tparam E - entity type
template<class E>
struct IsFlagComponent {
	/// @brief Actual predicate
	/// @tparam C - component type
	template<class C>
	struct Pred {
		static inline constexpr bool value = _details::ComponentTraits<C, E>::flag;
	};
};
} // namespace _details

/// @brief Domain view
/// @tparam E - entity type
/// @tparam Const - whether view is readonly
/// @tparam Include - TypeList with components included in view
/// @tparam Exclude - TypeList with components excluded in view
template<class E, bool Const, class Include, class Exclude>
class View;

/// @brief Domain's view
/// @tparam E - entity type
/// @tparam Const - whether view is readonly
/// @tparam Includes - components included in view
/// @tparam Excludes - components excluded in view
template<class E, bool Const, class... Includes, class... Excludes>
class View<E, Const, TypeList<Includes...>, TypeList<Excludes...>> {
public:

	/// @brief Entity traits
	using Traits = _details::EntityTraits<E>;
	/// @brief Entity type
	using EntityT = Traits::EntityT;
	/// @brief Actual domain type used
	using DomainT = std::conditional_t<Const, const Domain<E>, Domain<E>>;

	/// @brief TypeList with components included in view
	using Include = TypeList<Includes...>;
	/// @brief TypeList with components excluded in view
	using Exclude = TypeList<Excludes...>;
	/// @brief Include without flag-components
	using NoFlags = typename Include::template eraseIf<_details::IsFlagComponent<EntityT>::template Pred>;

	/// @brief Count of included component types
	static inline constexpr size_t includeCount = Include::length;
	/// @brief Count of excluded component types
	static inline constexpr size_t excludeCount = Exclude::length;
	/// @brief Whether view is readonly
	static inline constexpr bool readonly = Const;

	/// @brief Checks if entity is contained by view
	/// @param entity - entity to check
	bool contains(const EntityT entity) const noexcept;

	/// @brief Recomputes entities in view
	/// @brief Needed if at least one component is in-place
	View& refresh() noexcept;

	/// @brief Returns std::tuple with included components, excluding flag-components
	/// @param entity - entity to get components from
	auto get(const EntityT entity) noexcept requires(!Const);
	/// @brief Returns std::tuple with readonly included components, excluding flag-components
	/// @param entity - entity to get components from
	auto get(const EntityT entity) const noexcept;
	/// @brief Returns std::tuple with included components, including flag-components
	/// @param entity - entity to get components from
	auto getAll(const EntityT entity) noexcept requires(!Const);
	/// @brief Returns std::tuple with readonly included components, including flag-components
	/// @param entity - entity to get components from
	auto getAll(const EntityT entity) const noexcept;
	/// @brief Returns std::tuple with given components
	/// @tparam Cs - components to get
	/// @param entity - entity to get components from
	template<class... Cs>
	auto get(const EntityT entity) noexcept requires(!Const);
	/// @brief Returns std::tuple with given readonly components
	/// @tparam Cs - components to get
	/// @param entity - entity to get components from
	template<class... Cs>
	auto get(const EntityT entity) const noexcept;

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
	auto begin() noexcept requires(!Const);
	/// @brief Returns iterator to beginning of view
	const auto begin() const noexcept;
	/// @brief Returns iterator to beginning of view
	const auto cbegin() const noexcept;
	/// @brief Returns iterator to end of view
	auto end() noexcept requires(!Const);
	/// @brief Returns iterator to end of view
	const auto end() const noexcept;
	/// @brief Returns iterator to end of view
	const auto cend() const noexcept;

private:

	friend Domain<E>;

	using CCPoolPtr =
		std::conditional_t<Const, const _details::CommonComponentPool<E>*, _details::CommonComponentPool<E>*>;
	template<class C>
	using CPoolPtr = std::conditional_t<
		Const,
		const ComponentPool<std::remove_const_t<C>, E>*,
		ComponentPool<std::remove_const_t<C>, E>*>;

	size_t _minInclude() const noexcept;

	template<class C>
	auto getAsTuple(const E entity) noexcept requires(!Const);
	template<class C>
	auto getAsTuple(const E entity) const noexcept;
	template<class... Cs>
	auto getByTL(const E entity, TypeList<Cs...>) noexcept requires(!Const);
	template<class... Cs>
	auto getByTL(const E entity, TypeList<Cs...>) const noexcept;

	// expected type of entities view
	using EntitesViewT = decltype(std::views::filter(
		std::declval<const _details::CommonComponentPool<E>>()._entitiesForView(),
		std::bind(&View::contains, std::declval<const View*>(), std::placeholders::_1)
	));

	View(DomainT* domain) noexcept;

	std::array<CCPoolPtr, includeCount> _includedCPools;
	std::array<CCPoolPtr, excludeCount> _excludedCPools;
	size_t _minIdx;
	EntitesViewT _entities;
};

/// @brief Domain's exclude-only view
/// @tparam E - entity type
/// @tparam Const - whether view is readonly
/// @tparam Excludes - components excluded in view
template<class E, bool Const, class... Excludes>
class View<E, Const, TypeList<>, TypeList<Excludes...>> {
public:

	/// @brief Entity traits
	using Traits = _details::EntityTraits<E>;
	/// @brief Entity type
	using EntityT = Traits::EntityT;
	/// @brief Actual domain type used

	/// @brief TypeList with components included in view
	using Include = TypeList<>;
	/// @brief TypeList with components excluded in view
	using Exclude = TypeList<Excludes...>;
	/// @brief Include without flag-components
	using DomainT = std::conditional_t<Const, const Domain<E>, Domain<E>>;

	/// @brief Count of included component types
	static inline constexpr size_t includeCount = TypeList<>::length;
	/// @brief Count of excluded component types
	static inline constexpr size_t excludeCount = TypeList<Excludes...>::length;
	/// @brief Whether view is readonly
	static inline constexpr bool readonly = Const;

	/// @brief Checks if entity is contained by view
	/// @param entity - entity to check
	bool contains(const EntityT entity) const noexcept;

	/// @brief Recomputes entities in view
	/// @brief Needed if at least one component is in-place
	View& refresh() noexcept;

	/// @brief Returns std::tuple with given components (may be out of view)
	/// @tparam Cs - components to get
	/// @param entity - entity to get components from
	template<class... Cs>
	auto get(const EntityT entity) noexcept requires(!Const);
	/// @brief Returns std::tuple with given readonly components (may be out of view)
	/// @tparam Cs - components to get
	/// @param entity - entity to get components from
	template<class... Cs>
	auto get(const EntityT entity) const noexcept;

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
	auto begin() noexcept requires(!Const);
	/// @brief Returns iterator to beginning of view
	const auto begin() const noexcept;
	/// @brief Returns iterator to beginning of view
	const auto cbegin() const noexcept;
	/// @brief Returns iterator to end of view
	auto end() noexcept requires(!Const);
	/// @brief Returns iterator to end of view
	const auto end() const noexcept;
	/// @brief Returns iterator to end of view
	const auto cend() const noexcept;

private:

	friend Domain<E>;

	// filtering function
	bool _containsNoCheck(const E entity) const noexcept;

	using CCPoolPtr =
		std::conditional_t<Const, const _details::CommonComponentPool<E>*, _details::CommonComponentPool<E>*>;
	template<class C>
	using CPoolPtr = std::conditional_t<
		Const,
		const ComponentPool<std::remove_const_t<C>, E>*,
		ComponentPool<std::remove_const_t<C>, E>*>;

	// expected type of entities view
	using EntitesViewT = decltype(std::views::filter(
		std::declval<const Domain<E>>().entities(),
		std::bind(&View::_containsNoCheck, std::declval<const View*>(), std::placeholders::_1)
	));

	View(DomainT* domain) noexcept;

	std::array<CCPoolPtr, excludeCount> _excludedCPools;
	EntitesViewT _entities;
	DomainT* _domain;
};

} // namespace arch::ecs

#include "View.hpp"
