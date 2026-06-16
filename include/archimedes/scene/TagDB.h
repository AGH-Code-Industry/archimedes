#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include <archimedes/ecs/Entity.h>

namespace arch::scene {

/// @brief Contains set of entities of given tag
/// @details TagDB is the owner of tag values
struct TagDB {
	/// @brief Hashing functor
	struct StrHash: public std::hash<std::string_view> {
		/// @brief Enables unordered_map to hash string as string_view
		using is_transparent = void; // NOLINT
	};

	/// @brief Comparator
	struct StrEq: public std::equal_to<std::string_view> {
		/// @brief Enables unordered_map to compare string and string_view
		using is_transparent = void; // NOLINT
	};

	/// @brief Map with tags and tagged entities
	/// @details Owns tag values
	std::unordered_map<std::string, std::unordered_set<ecs::Entity>, StrHash, StrEq> value;
};

} // namespace arch::scene
