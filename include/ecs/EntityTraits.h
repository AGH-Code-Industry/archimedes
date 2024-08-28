#pragma once

#include <limits>

#include "EntitySpecs.h"

namespace arch::ecs::_details { /* NOLINT */

// template<class ES>
// struct EntityTraits {
//	using Specs = ES;
// };

template<class E>
struct EntityTraits {
	using Specs = EntitySpecs<E>;

	using EntityT = typename Specs::EntityT;
	using EntityRawT = std::underlying_type_t<EntityT>;
	using IdT = typename Specs::IdT;
	using VersionT = typename Specs::VersionT;

	struct Entity {
		using Type = EntityT;
		using RawT = EntityRawT;

		static inline constexpr size_t length = sizeof(Type) * 8;
		static inline constexpr Type null = (EntityT)std::numeric_limits<RawT>::max();

		static inline Type fromParts(IdT id, VersionT ver) noexcept;
		static inline Type fromRawParts(RawT id, RawT ver) noexcept;
		static inline Type fromOthers(Type id, Type ver) noexcept;
		static inline RawT toRaw(Type entity) noexcept;
	};

	struct Id {
		using Type = IdT;

		static inline constexpr size_t length = Specs::idLength;
		static inline constexpr EntityRawT mask = ~(std::numeric_limits<EntityRawT>::max() << length);
		static inline constexpr EntityRawT maxPart = (EntityRawT)mask - 1;
		static inline constexpr Type max = (Type)maxPart;
		static inline constexpr Type null = max + 1;
		static inline constexpr EntityRawT nullPart = mask;

		static inline Type part(EntityT entity) noexcept;
		static inline EntityRawT asRawPart(Type id) noexcept;
		static inline EntityRawT rawPart(EntityT entity) noexcept;
		static inline bool hasNull(EntityT entity) noexcept;
		static inline EntityT withNull(EntityT entity) noexcept;
		static inline void swap(EntityT& entity1, EntityT& entity2) noexcept;
	};

	struct Version {
		using Type = VersionT;

		static inline constexpr size_t length = Entity::length - Specs::idLength;
		static inline constexpr EntityRawT mask = ~(std::numeric_limits<EntityRawT>::max() >> length);
		static inline constexpr EntityRawT maxPart = ((mask - 1) & mask);
		static inline constexpr Type max = (Type)(maxPart >> (Entity::length - length));
		static inline constexpr Type null = max + 1;
		static inline constexpr EntityRawT nullPart = mask;

		static inline Type part(EntityT entity) noexcept;
		static inline EntityRawT asRawPart(Type version) noexcept;
		static inline EntityRawT rawPart(EntityT entity) noexcept;
		static inline bool hasNull(EntityT entity) noexcept;
		static inline EntityT withNext(EntityT entity) noexcept;
		static inline Type next(EntityT entity) noexcept;
		static inline EntityT withNull(EntityT entity) noexcept;
		static inline void swap(EntityT& entity1, EntityT& entity2) noexcept;
	};
};

} // namespace arch::ecs::_details

#include "EntityTraits.hpp"
