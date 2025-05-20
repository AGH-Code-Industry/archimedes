#pragma once

#include <type_traits>

#include "ArchMath.h"
#include "Entity.h"

namespace arch::ecs {

/// @brief Entity specification
struct EntitySpecs {
	/// @brief Type of entity
	using EntityT = Entity;

	/// @brief Type of id
	using IdT = std::underlying_type_t<EntityT>;
	/// @brief Type of version
	using VersionT = uint16_t;

	/// @brief Bit length of id
	static inline constexpr size_t idLength = 48;
	/// @brief Size of sparse pages
	static inline constexpr size_t pageSize = 1'024 * 8;
};

} // namespace arch::ecs
