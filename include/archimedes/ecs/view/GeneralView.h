#pragma once

#include "ViewFwd.h"
//
#include "../ComponentPool.h"
#include "ExcludeT.h"
#include "GeneralViewIterator.h"
#include <archimedes/utils/CallableTraits.h>
#include <archimedes/utils/TypeList.h>

namespace arch::ecs {

class Domain;

/// @brief General view
template<class... Includes, class... Excludes>
requires(sizeof...(Includes) != 0 || sizeof...(Excludes) != 0)
class View<TypeList<Includes...>, TypeList<Excludes...>>:
	public std::ranges::view_interface<View<TypeList<Includes...>, TypeList<Excludes...>>> {
public:

	/// @brief Iterator type
	using Iterator = ViewIterator<TypeList<Includes...>, TypeList<Excludes...>>;

	/// @brief Included types
	static constexpr auto includes = typelist<Includes...>;
	/// @brief Excluded types
	static constexpr auto excludes = typelist<Excludes...>;

	// Overlap check
	static_assert(
		!typelist<std::remove_const_t<Includes>...>.containsAny(typelist<std::remove_const_t<Excludes>...>),
		"Includes and Excludes overlap"
	);

	/// @brief Default constructor
	View() noexcept = default;
	/// @brief Constructor
	View(Domain& domain) noexcept;

	/*~View() {
		this->_cpools = {};
		this->_cpoolsExcl = {};
		this->_minCpoolIdx = {};
	}*/

	/// @brief Begin iterator
	Iterator begin() const noexcept;
	/// @brief End iterator
	Iterator end() const noexcept;

	/// @brief Calls fn for each entity
	/// @details Accepts those callables:
	/// @details - callables, where args are a subset of offered components
	/// @details - callables, where first arg is ecs::Entity, rest of args are a subset of offered components
	/// @details - template callables, where args exactly match offered components
	/// @details - template callables, where first arg is ecs::Entity, rest of args exactly match offered components
	void forEach(auto&& fn);

	/// @brief Returns wrapper view that obtains each component for each entity
	/// @details Usage: for(auto [c1, c2, ...] : view.comps()) { ... }
	auto comps() noexcept;
	/// @brief Returns wrapper view that obtains each component for each entity, also returning entity
	/// @details Usage: for(auto [entity, c1, c2, ...] : view.comps()) { ... }
	auto entityComps() noexcept;

	/// @brief Obtains components of given entity
	/// @details Entity must contain all components
	auto comps(const Entity entity) noexcept;
	/// @brief Obtains components of given entity (as optional)
	auto compsOpt(const Entity entity) noexcept;
	/// @brief Checks if entity is withing this view
	bool contains(const Entity entity) const noexcept;

	/// @brief Returns index of ComponentPool which has minimal components
	u32 minCPool() const noexcept;

private:

	using Traits = _details::EntityTraits::Version;

	// Non-flag components (including consts)
	static consteval auto _availableComponents();
	// includes.eraseIf<IsFlag>()
	static consteval auto _nonFlags();

	// executes for-each for wanted components
	template<bool WithEntity, class... Cs>
	void _forEach(auto&& fn, TypeList<Cs...> wanted);
	// executes comps for wanted components
	template<class... Cs>
	auto _comps(TypeList<Cs...> wanted) noexcept;
	// executes entityComps for wanted components
	template<class... Cs>
	auto _entityComps(TypeList<Cs...> wanted) noexcept;
	// executes comps(entity) for wanted components
	template<class... Cs>
	auto _comps(const Entity entity, TypeList<Cs...> wanted) noexcept;
	// executes compsOpt(entity) for wanted components
	template<class... Cs>
	auto _compsOpt(const Entity entity, TypeList<Cs...> wanted) noexcept
		-> std::optional<decltype(_comps(entity, wanted))>;

	template<class, class>
	friend class ViewIterator;

	std::array<_details::CommonComponentPool*, includes.size()> _cpools{};
	std::array<const _details::CommonComponentPool*, excludes.size()> _cpoolsExcl{};
	u32 _minCpoolIdx = (u32)-1;
};

} // namespace arch::ecs

#include "GeneralView.hpp"
