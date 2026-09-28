#pragma once

#include "ComponentBases.h"

namespace arch::ecs::_details { // NOLINT

/// @brief Traits checking if T is a flag component
template<class T>
struct IsFlag {
	static constexpr bool value = std::is_base_of_v<ecs::FlagComponent, T>;
};

} // namespace arch::ecs::_details
