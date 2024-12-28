#pragma once

#include <string>
#include <unordered_set>

#include <ecs/Entity.h>

namespace arch::scene {

struct Tags {
	std::unordered_set<std::string_view> value;
};

} // namespace arch::scene
