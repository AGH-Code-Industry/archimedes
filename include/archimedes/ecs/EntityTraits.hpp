#include "EntityTraits.h"
//
#include <archimedes/ArchMath.h>

namespace arch::ecs::_details {

constexpr EntityTraits::Ent::Type EntityTraits::Ent::fromParts(const IdT id, const VersionT ver) noexcept {
	return fromRawParts(Id::asRawPart(id), Version::asRawPart(ver));
}

constexpr EntityTraits::Ent::Type EntityTraits::Ent::fromRawParts(const RawT id, const RawT ver) noexcept {
	return Type(id | ver);
}

constexpr EntityTraits::Ent::Type EntityTraits::Ent::fromOthers(const Type id, const Type ver) noexcept {
	return fromRawParts(Id::rawPart(id), Version::rawPart(ver));
}

constexpr EntityTraits::Ent::RawT EntityTraits::Ent::toRaw(const Type entity) noexcept {
	return (RawT)entity;
}

constexpr EntityTraits::Id::Type EntityTraits::Id::part(const EntityT entity) noexcept {
	return rawPart(entity);
}

constexpr EntityTraits::EntityRawT EntityTraits::Id::asRawPart(const Type id) noexcept {
	return (EntityRawT)id & mask;
}

constexpr EntityTraits::EntityRawT EntityTraits::Id::rawPart(const EntityT entity) noexcept {
	return Ent::toRaw(entity) & mask;
}

constexpr bool EntityTraits::Id::hasNull(const EntityT entity) noexcept {
	return (Ent::toRaw(entity) & mask) == nullRaw;
}

constexpr EntityTraits::EntityT EntityTraits::Id::withNull(const EntityT entity) noexcept {
	return Ent::fromRawParts(nullRaw, Version::rawPart(entity));
}

constexpr void EntityTraits::Id::swap(EntityT& entity1, EntityT& entity2) noexcept {
	const auto id1 = rawPart(entity1);
	entity1 = Ent::fromRawParts(rawPart(entity2), Version::rawPart(entity1));
	entity2 = Ent::fromRawParts(id1, Version::rawPart(entity2));
}

constexpr bool EntityTraits::Id::equal(const EntityT entity1, const EntityT entity2) noexcept {
	return rawPart(entity1) == rawPart(entity2);
}

constexpr EntityTraits::Version::Type EntityTraits::Version::part(const EntityT entity) noexcept {
	return Type(rawPart(entity) >> Id::length);
}

constexpr EntityTraits::EntityRawT EntityTraits::Version::asRawPart(const Type version) noexcept {
	return (EntityRawT)version << Id::length;
}

constexpr EntityTraits::EntityRawT EntityTraits::Version::rawPart(const EntityT entity) noexcept {
	return Ent::toRaw(entity) & mask;
}

constexpr bool EntityTraits::Version::hasNull(const EntityT entity) noexcept {
	return (Ent::toRaw(entity) & mask) == nullRaw;
}

constexpr bool EntityTraits::Version::hasNotNull(const EntityT entity) noexcept {
	return (Ent::toRaw(entity) & mask) != nullRaw;
}

constexpr EntityTraits::EntityT EntityTraits::Version::withNext(const EntityT entity) noexcept {
	return Ent::fromParts(Id::part(entity), next(entity));
}

constexpr EntityTraits::Version::Type EntityTraits::Version::next(const EntityT entity) noexcept {
	const auto newVersion = part(entity) + 1;

	return newVersion + (newVersion == null);
}

constexpr EntityTraits::EntityT EntityTraits::Version::withNull(const EntityT entity) noexcept {
	return Ent::fromRawParts(Id::rawPart(entity), nullRaw);
}

constexpr void EntityTraits::Version::swap(EntityT& entity1, EntityT& entity2) noexcept {
	const auto ver1 = rawPart(entity1);
	entity1 = Ent::fromRawParts(EntityTraits::Id::rawPart(entity1), rawPart(entity2));
	entity2 = Ent::fromRawParts(EntityTraits::Id::rawPart(entity2), ver1);
}

constexpr bool EntityTraits::Version::equal(const EntityT entity1, const EntityT entity2) noexcept {
	return rawPart(entity1) == rawPart(entity2);
}

} // namespace arch::ecs::_details
