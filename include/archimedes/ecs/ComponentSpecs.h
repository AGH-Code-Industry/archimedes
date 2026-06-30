#pragma once

#include "ComponentBases.h"

namespace arch::ecs {

/// @brief Helper class to obtain pageSize from base class
template<class T>
struct GetPageSize {
	static constexpr size_t value = 1'024;
};

/// @brief Helper class to obtain page size from base class
template<class T>
requires(std::is_base_of_v<_details::ComponentPageSizeBase, T>)
struct GetPageSize<T> {
	/// @brief Helper function that extracts PageSize from ComponentPageSize-derived classes
	template<size_t PageSize>
	static auto extract(const ComponentPageSize<PageSize>&) -> std::integral_constant<size_t, PageSize>;

	static constexpr size_t value = decltype(extract(std::declval<T>()))::value;
};

/// @brief Contains basic specification of a component type:
/// @brief bool inPlace (false by default) - whether components should not be moved by basic operations, guarantees
/// pointer stability
/// @brief size_t pageSize (1024 by default) - page size in component storage, must be a power of two
/// @tparam C - component type
template<class C>
struct ComponentSpecs {
	/// @brief Whether components should not be moved by basic operations
	static inline constexpr bool inPlace = std::is_base_of_v<InPlaceComponent, C> ||
		!(std::is_move_assignable_v<C> && std::is_move_constructible_v<C>);
	/// @brief Page size in component storage, 1024 by default
	static inline constexpr size_t pageSize = GetPageSize<C>::value;
	/// @brief Whether components are flag-components
	static inline constexpr bool flag = std::is_base_of_v<FlagComponent, C>;
};

} // namespace arch::ecs
