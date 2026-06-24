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

template<class... Includes, class... Excludes>
requires(sizeof...(Includes) != 0 || sizeof...(Excludes) != 0)
class View<TypeList<Includes...>, TypeList<Excludes...>>:
	public std::ranges::view_interface<View<TypeList<Includes...>, TypeList<Excludes...>>> {
public:

	using Iterator = ViewIterator<TypeList<Includes...>, TypeList<Excludes...>>;

	static constexpr auto includes = typelist<Includes...>;
	static constexpr auto excludes = typelist<Excludes...>;

	View() noexcept = default;
	View(Domain& domain) noexcept;

	Iterator begin() const noexcept;
	Iterator end() const noexcept;

	void forEach(auto&& fn);

	auto comps() noexcept;
	auto entityComps() noexcept;

	auto comps(const Entity entity) noexcept;
	bool contains(const Entity entity) const noexcept;

	u32 minCPool() const noexcept;

private:

	using Traits = _details::EntityTraits::Version;

	static consteval auto _availableComponents();

	template<bool WithEntity, class... Cs>
	void _forEach(auto&& fn, TypeList<Cs...> wanted);
	template<class... Cs>
	auto _comps(TypeList<Cs...> wanted) noexcept;
	template<class... Cs>
	auto _entityComps(TypeList<Cs...> wanted) noexcept;
	template<class... Cs>
	auto _comps(const Entity entity, TypeList<Cs...> wanted) noexcept;

	template<class, class>
	friend class ViewIterator;

	std::array<_details::CommonComponentPool*, includes.size()> _cpools{};
	std::array<const _details::CommonComponentPool*, excludes.size()> _cpoolsExcl{};
	u32 _minCpoolIdx = (u32)-1;
};

} // namespace arch::ecs

#include "GeneralView.hpp"
