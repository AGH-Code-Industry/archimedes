#pragma once

#include "ComponentTraits.h"

namespace arch::ecs {

template<class C>
struct ComponentInfo {
	/// @brief Type of component
	using ComponentT = C;

	/// @brief Component Specifications
	using Specs = _details::ComponentTraits<std::remove_const_t<C>>::Specs;

	/// @brief Whether component is a flag component,
	static inline constexpr bool flag = _details::ComponentTraits<std::remove_const_t<C>>::flag;

	/// @brief Whether component is marked as in-place
	static inline constexpr bool inPlace = _details::ComponentTraits<std::remove_const_t<C>>::inPlace;
	/// @brief Whether component is movable
	static inline constexpr bool movable = _details::ComponentTraits<std::remove_const_t<C>>::movable;
	/// @brief Component page size
	static inline constexpr size_t pageSize = _details::ComponentTraits<std::remove_const_t<C>>::pageSize;

	/// @brief Size of component in bytes
	static inline constexpr size_t size = sizeof(ComponentT);
	/// @brief Alignment of component
	static inline constexpr size_t alignment = alignof(ComponentT);
};

} // namespace arch::ecs

namespace arch {

using ecs::ComponentInfo;

}
