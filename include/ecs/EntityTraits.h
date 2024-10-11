#pragma once

#include <bit>
#include <limits>

#include "EntitySpecs.h"

namespace arch::ecs::_details { /* NOLINT */

/// @brief Contains info, constants and operations on entities, their ids and versions
/// @details All data extracted from EntitySpecs
/// @details 'part' of entity - abstract value describing part of entity
/// @details 'raw part' of entity - 'part' of entity as held in memory, in general 'part' != 'raw part'
/// @tparam E - entity type
template<class E>
struct EntityTraits {
	/// @brief Entity specifications
	using Specs = EntitySpecs<E>;

	/// @brief Entity type
	using EntityT = typename Specs::EntityT;
	/// @brief Entity underlying type
	using EntityRawT = std::underlying_type_t<EntityT>;
	/// @brief Entity id type
	using IdT = typename Specs::IdT;
	/// @brief Entity version type
	using VersionT = typename Specs::VersionT;

	/// @brief Size of sparse pages
	static inline constexpr size_t pageSize = Specs::pageSize;
	static_assert(std::popcount(pageSize) == 1, "pageSize must be a power of 2");

	/// @brief Entity section
	struct Entity {
		/// @brief Entity type
		using Type = EntityT;
		/// @brief Entity underlying type
		using RawT = EntityRawT;

		/// @brief Bit length of entity
		static inline constexpr size_t length = sizeof(Type) * 8;
		/// @brief Null entity
		static inline constexpr Type null = (EntityT)std::numeric_limits<RawT>::max();

		/// @brief Constructs entity from its parts
		/// @param id - id of entity
		/// @param ver - version of entity
		static inline Type fromParts(const IdT id, const VersionT ver) noexcept;
		/// @brief Constructs entity from its raw parts
		/// @param id - id part of entity
		/// @param ver - version part of entity
		static inline Type fromRawParts(const RawT id, const RawT ver) noexcept;
		/// @brief Constructs entity from others
		/// @param id - entity to extract id from
		/// @param ver - entity to extract version from
		static inline Type fromOthers(const Type id, const Type ver) noexcept;
		/// @brief Obtains entity as underlying type
		/// @param entity - entity to convert
		static inline RawT toRaw(const Type entity) noexcept;
	};

	/// @brief Id section
	struct Id {
		/// @brief Id type
		using Type = IdT;

		/// @brief Bit length of id
		static inline constexpr size_t length = Specs::idLength;
		/// @brief Bit mask of id
		static inline constexpr EntityRawT mask = ~(std::numeric_limits<EntityRawT>::max() << length);
		/// @brief Max value of id as raw part
		static inline constexpr EntityRawT maxRaw = (EntityRawT)mask - 1;
		/// @brief Max value of id as part
		static inline constexpr Type max = (Type)maxRaw;
		/// @brief Null id (part)
		static inline constexpr Type null = max + 1;
		/// @brief Null id (raw part)
		static inline constexpr EntityRawT nullRaw = mask;

		/// @brief Obtains id part from given entity
		static inline Type part(const EntityT entity) noexcept;
		/// @brief Obtains id raw part from given entity
		static inline EntityRawT rawPart(const EntityT entity) noexcept;
		/// @brief Converts id part to raw part
		static inline EntityRawT asRawPart(const Type id) noexcept;
		/// @brief Checks if entity has null id
		static inline bool hasNull(const EntityT entity) noexcept;
		/// @brief Creates entity with null id and version of given entity
		static inline EntityT withNull(const EntityT entity) noexcept;
		/// @brief Swaps ids of two entities
		static inline void swap(EntityT& entity1, EntityT& entity2) noexcept;
		/// @brief Compares ids of 2 given entities
		static inline bool equal(const EntityT entity1, const EntityT entity2) noexcept;
	};

	/// @brief Version section
	struct Version {
		/// @brief Version type
		using Type = VersionT;

		/// @brief Bit length of version
		static inline constexpr size_t length = Entity::length - Specs::idLength;
		/// @brief Bit mask of version
		static inline constexpr EntityRawT mask = ~(std::numeric_limits<EntityRawT>::max() >> length);
		/// @brief Max value of version as raw part
		static inline constexpr EntityRawT maxRaw = ((mask - 1) & mask);
		/// @brief Max value of version as part
		static inline constexpr Type max = (Type)(maxRaw >> (Entity::length - length));
		/// @brief Null version (part)
		static inline constexpr Type null = max + 1;
		/// @brief Null version (raw part)
		static inline constexpr EntityRawT nullRaw = mask;

		/// @brief Obtains version part from given entity
		static inline Type part(const EntityT entity) noexcept;
		/// @brief Obtains version raw part from given entity
		static inline EntityRawT asRawPart(const Type version) noexcept;
		/// @brief Converts version part to raw part
		static inline EntityRawT rawPart(const EntityT entity) noexcept;
		/// @brief Checks if entity has null version
		static inline bool hasNull(const EntityT entity) noexcept;
		/// @brief Checks if entity has != null version
		static inline bool hasNotNull(const EntityT entity) noexcept;
		/// @brief Creates entity with next version and id of given entity
		static inline EntityT withNext(const EntityT entity) noexcept;
		/// @brief Gets next version of given entity
		static inline Type next(const EntityT entity) noexcept;
		/// @brief Creates entity with null version and id of given entity
		static inline EntityT withNull(const EntityT entity) noexcept;
		/// @brief Swaps versions of two entities
		static inline void swap(EntityT& entity1, EntityT& entity2) noexcept;
		/// @brief Compares versions of 2 given entities
		static inline bool equal(const EntityT entity1, const EntityT entity2) noexcept;
	};
};

extern template class EntityTraits<e32>;
extern template class EntityTraits<e64>;

} // namespace arch::ecs::_details

#include "EntityTraits.hpp"

