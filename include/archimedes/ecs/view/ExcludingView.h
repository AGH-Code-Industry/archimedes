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

template<class... Excludes>
requires(sizeof...(Excludes) != 0)
class View<TypeList<>, TypeList<Excludes...>>:
	public std::ranges::view_interface<View<TypeList<>, TypeList<Excludes...>>> {
public:

	using Iterator = ViewIterator<TypeList<>, TypeList<Excludes...>>;

	static constexpr auto includes = typelist<>;
	static constexpr auto excludes = typelist<Excludes...>;

	View() noexcept = default;
	View(Domain& domain) noexcept;

	Iterator begin() const noexcept;
	Iterator end() const noexcept;

	void forEach(auto&& fn);

	bool contains(const Entity entity) const noexcept;

private:

	using Traits = _details::EntityTraits::Version;

	template<class, class>
	friend class ViewIterator;

	Domain* _domain = nullptr;
	std::array<const _details::CommonComponentPool*, excludes.size()> _cpoolsExcl{};
};

} // namespace arch::ecs

#include "ExcludingView.hpp"
