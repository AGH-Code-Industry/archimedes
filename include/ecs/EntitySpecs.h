#pragma once

#include <type_traits>

#include "ArchMath.h"
#include "Entity.h"

namespace arch::ecs {

/// @brief Automatic entity specification
/// @tparam E - entity type
/// @tparam V - version type
/// @tparam IdLen - bit length of id
template<class E, class V, size_t IdLen, size_t PageSize = 1'024>
struct AutoEntitySpecs {
	/// @brief Type of entity
	using EntityT = E;

	/// @brief Type of id
	using IdT = std::underlying_type_t<E>;
	/// @brief Type of version
	using VersionT = V;

	/// @brief Bit length of id
	static inline constexpr size_t idLength = IdLen;
	/// @brief Size of sparse pages
	static inline constexpr size_t pageSize = PageSize;
};

/// @brief Contains specification of an entity, specialize template to create your own
/// @brief Must have the same members as AutoEntitySpecs (usage by inheritance is recommended)
/// @tparam E - entity type
template<class E>
struct EntitySpecs {
	static_assert("EntitySpecs not specialized for given type");
};

/// @brief EntitySpecs specialization for 32-bit entity
/// @sa arch::ecs::e32
template<>
struct EntitySpecs<e32>: AutoEntitySpecs<e32, u8, 24, 1'024> {};

template<>
/// @brief EntitySpecs specialization for 64-bit entity
/// @sa arch::ecs::e64
struct EntitySpecs<e64>: AutoEntitySpecs<e64, u16, 48, 1'024> {};

} // namespace arch::ecs
