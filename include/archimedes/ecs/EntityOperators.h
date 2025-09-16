#pragma once

#include "Entity.h"
#include "EntityTraits.h"

namespace arch::ecs {

/// @brief Returns ID part of entity
/// @param entity - entity to obtain ID of
constexpr _details::EntityTraits::IdT operator*(Entity entity) noexcept;

/// @brief Returns Version part of entity
/// @param entity - entity to obtain ID of
constexpr _details::EntityTraits::VersionT operator~(Entity entity) noexcept;

} // namespace arch::ecs

#include "EntityOperators.hpp"
