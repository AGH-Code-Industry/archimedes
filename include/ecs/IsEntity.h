#pragma once

#include <concepts>

#include "Entity.h"
#include <tUtils/TypeList.h>

namespace arch::ecs::_details {

template<class T>
struct IsEntity {
	static constexpr bool value = std::same_as<T, Entity> || std::same_as<T, const Entity> ||
		std::same_as<T, const Entity&>;
};

} // namespace arch::ecs::_details
