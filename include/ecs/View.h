#pragma once

#include <ranges>
#include <type_traits>

#include "ComponentPool.h"
#include "ContainChecks.h"
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
inline auto getAsTuple(Domain<E>& domain, const E entity) noexcept;
/// @brief Returns given readonly component wrapped in std::tuple
/// @tparam E - entity type
/// @tparam C - component type
/// @param domain - domain to get component from
/// @param entity - entity to get component from
template<class E, class C>
inline auto getAsTuple(const Domain<E>& domain, const E entity) noexcept;
/// @brief Returns std::tuple with specified components from given entity
/// @tparam E - entity type
/// @tparam Includes - component types
/// @param domain - domain to get components from
/// @param entity - entity to get components from
/// @param <unnamed> - TypeString instance to help in deduction
template<class E, class... Includes>
inline auto getByTS(Domain<E>& domain, const E entity, TypeString<Includes...>) noexcept;
/// @brief Returns std::tuple with specified components from given entity
/// @tparam E - entity type
/// @tparam Includes - component types
/// @param domain - domain to get components from
/// @param entity - entity to get components from
/// @param <unnamed> - TypeString instance to help in deduction
template<class E, class... Includes>
inline auto getByTS(const Domain<E>& domain, const E entity, TypeString<Includes...>) noexcept;

/// @brief Predicate wrapper for TypeString to filter out flag-components
/// @tparam E - entity type
template<class E>
struct IsFlagComponent {
	/// @brief Actual predicate
	/// @tparam C - component type
	template<class C>
	using Pred = std::bool_constant<_details::ComponentTraits<C, E>::flag>;
};
} // namespace _details

/// @brief Domain view
/// @tparam E - entity type
/// @tparam Const - whether view is readonly
/// @tparam Include - TypeString with components included in view
/// @tparam Exclude - TypeString with components excluded in view
template<class E, bool Const, class Include, class Exclude>
class View;

/// @brief Domain's view
/// @tparam E - entity type
/// @tparam Const - whether view is readonly
/// @tparam Includes - components included in view
/// @tparam Excludes - components excluded in view
template<class E, bool Const, class... Includes, class... Excludes>
class View<E, Const, TypeString<Includes...>, TypeString<Excludes...>> {
public:

	/// @brief Entity traits
	using Traits = _details::EntityTraits<E>;
	/// @brief Entity type
	using EntityT = Traits::EntityT;
	/// @brief Actual domain type used
	using DomainT = std::conditional_t<Const, const Domain<E>, Domain<E>>;

	/// @brief TypeString with components included in view
	using Include = TypeString<Includes...>;
	/// @brief TypeString with components excluded in view
	using Exclude = TypeString<Excludes...>;
	/// @brief Include without flag-components
	using NoFlags = typename Include::template eraseIf<typename _details::IsFlagComponent<EntityT>::Pred>;

	/// @brief Count of included component types
	static inline constexpr size_t includeCount = Include::length;
	/// @brief Count of excluded component types
	static inline constexpr size_t excludeCount = Exclude::length;
	/// @brief Whether view is readonly
	static inline constexpr bool readonly = Const;

	/// @brief Checks if entity is contained by view
	/// @param entity - entity to check
	bool contains(const EntityT entity) const noexcept;

	/// @brief Returns std::tuple with included components
	/// @param entity - entity to get components from
	auto get(const EntityT entity) noexcept requires(!Const);
	/// @brief Returns std::tuple with readonly included components
	/// @param entity - entity to get components from
	auto get(const EntityT entity) const noexcept;
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

	// filtering function
	static bool _filterFn(const Domain<E>& domain, const E entity) noexcept;

	// expected type of entities view
	using EntitesViewT = decltype(std::views::filter(
		((const _details::CommonComponentPool<E>*)nullptr)->_entitiesForView(),
		std::bind(_filterFn, std::cref(*((const Domain<E>*)nullptr)), std::placeholders::_1)
	));

	View(DomainT* domain, const _details::CommonComponentPool<E>& minCPool) noexcept;

	DomainT* _domain;
	EntitesViewT _entities;
};

/// @brief Domain's exclude-only view
/// @tparam E - entity type
/// @tparam Const - whether view is readonly
/// @tparam Excludes - components excluded in view
template<class E, bool Const, class... Excludes>
class View<E, Const, TypeString<>, TypeString<Excludes...>> {
public:

	/// @brief Entity traits
	using Traits = _details::EntityTraits<E>;
	/// @brief Entity type
	using EntityT = Traits::EntityT;
	/// @brief Actual domain type used

	/// @brief TypeString with components included in view
	using Include = TypeString<>;
	/// @brief TypeString with components excluded in view
	using Exclude = TypeString<Excludes...>;
	/// @brief Include without flag-components
	using DomainT = std::conditional_t<Const, const Domain<E>, Domain<E>>;

	/// @brief Count of included component types
	static inline constexpr size_t includeCount = TypeString<>::length;
	/// @brief Count of excluded component types
	static inline constexpr size_t excludeCount = TypeString<Excludes...>::length;
	/// @brief Whether view is readonly
	static inline constexpr bool readonly = Const;

	/// @brief Checks if entity is contained by view
	/// @param entity - entity to check
	bool contains(const EntityT entity) const noexcept;

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
	static bool _filterFn(const Domain<E>& domain, const E entity) noexcept;

	// expected type of entities view
	using EntitesViewT = decltype(std::views::filter(
		((const Domain<E>*)nullptr)->entities(),
		std::bind(_filterFn, std::cref(*((const Domain<E>*)nullptr)), std::placeholders::_1)
	));

	View(DomainT* domain) noexcept;

	DomainT* _domain;
	EntitesViewT _entities;
};

} // namespace arch::ecs

#include "View.hpp"
