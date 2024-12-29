#pragma once

#include <string>
#include <unordered_set>

#include <ecs/Entity.h>

namespace arch::scene {

/// @brief Contains tags of entity
struct Tags {
	/// @brief Set of tags
	/// @details Does not own tag values
	std::unordered_set<std::string_view> value;
};

} // namespace arch::scene
