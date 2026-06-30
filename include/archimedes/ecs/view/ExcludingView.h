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

/// @brief Exclude-only view
template<class... Excludes>
requires(sizeof...(Excludes) != 0)
class View<TypeList<>, TypeList<Excludes...>>:
	public std::ranges::view_interface<View<TypeList<>, TypeList<Excludes...>>> {
public:

	/// @brief Iterator type
	using Iterator = ViewIterator<TypeList<>, TypeList<Excludes...>>;

	/// @brief Included types (emtpy)
	static constexpr auto includes = typelist<>;
	/// @brief Excluded types
	static constexpr auto excludes = typelist<Excludes...>;

	/// @brief Default constructor
	View() noexcept = default;
	/// @brief Constructor
	View(Domain& domain) noexcept;

	/// @brief Begin iterator
	Iterator begin() const noexcept;
	/// @brief End iterator
	Iterator end() const noexcept;

	/// @brief Calls fn for each entity
	void forEach(auto&& fn);

	/// @brief Checks if entity is withing this view
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
