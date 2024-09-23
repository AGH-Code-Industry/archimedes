#pragma once

#include "Flag.h"
#include "InClassComponentSpecs.h"

namespace arch::ecs {

/// @brief Contains basic specification of a component type:
/// @brief bool inPlace (false by default) - whether components should not be moved by basic operations, guarantees
/// pointer stability
/// @brief size_t pageSize (1024 by default) - page size in component storage, must be a power of two
/// @tparam C - component type
template<class C>
struct ComponentSpecs {
	/// @brief Whether components should not be moved by basic operations
	static inline constexpr bool inPlace = _details::AnyInClassInPlaceComponent<C> ||
		!(std::is_move_assignable_v<C> && std::is_move_constructible_v<C>);
	/// @brief Page size in component storage, 1024 by default
	static inline constexpr size_t pageSize =
		(_details::AnyInClassComponentPageSize<C> ? _details::InClassComponentPageSizeValue<C>::value : 1'024);
	/// @brief Whether components are flag-components
	static inline constexpr bool flag = _details::FlagComponent<C>;
};

} // namespace arch::ecs
