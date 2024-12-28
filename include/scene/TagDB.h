#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include <ecs/Entity.h>

namespace arch::scene {

struct TagDB {
	struct StrHash: public std::hash<std::string_view> {
		/// @brief Enables unordered_map to hash string as string_view
		using is_transparent = void; // NOLINT
	};

	struct StrEq: public std::equal_to<std::string_view> {
		/// @brief Enables unordered_map to compare string and string_view
		using is_transparent = void; // NOLINT
	};

	std::unordered_map<std::string, std::unordered_set<ecs::Entity>, StrHash, StrEq> value;
};

} // namespace arch::scene
