#pragma once

#include <concepts>

#include "Entity.h"
#include <archimedes/tUtils/TypeList.h>

namespace arch::ecs::_details {

/// @brief Trait checking if T is Entity, const Entity, or const Entity&
template<class T>
struct IsEntity {
	static constexpr bool value = std::same_as<T, Entity> || std::same_as<T, const Entity> ||
		std::same_as<T, const Entity&>;
};

} // namespace arch::ecs::_details
