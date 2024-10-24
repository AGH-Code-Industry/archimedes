#include "ArchMath.h"
#include <ecs/EntityTraits.h>

#define TRAITS EntityTraits
#define ENTITY TRAITS::Ent
#define ID TRAITS::Id
#define VERSION TRAITS::Version

namespace arch::ecs::_details {

ENTITY::Type ENTITY::fromParts(const IdT id, const VersionT ver) noexcept {
	return fromRawParts(Id::asRawPart(id), Version::asRawPart(ver));
}

ENTITY::Type ENTITY::fromRawParts(const RawT id, const RawT ver) noexcept {
	return Type(id | ver);
}

ENTITY::Type ENTITY::fromOthers(const Type id, const Type ver) noexcept {
	return fromRawParts(Id::rawPart(id), Version::rawPart(ver));
}

ENTITY::RawT ENTITY::toRaw(const Type entity) noexcept {
	return (RawT)entity;
}

ID::Type ID::part(const EntityT entity) noexcept {
	return { rawPart(entity) };
}

TRAITS::EntityRawT ID::asRawPart(const Type id) noexcept {
	return (EntityRawT)id & mask;
}

TRAITS::EntityRawT ID::rawPart(const EntityT entity) noexcept {
	return Ent::toRaw(entity) & mask;
}

bool ID::hasNull(const EntityT entity) noexcept {
	return (Ent::toRaw(entity) & mask) == nullRaw;
}

TRAITS::EntityT ID::withNull(const EntityT entity) noexcept {
	return Ent::fromRawParts(nullRaw, Version::rawPart(entity));
}

void ID::swap(EntityT& entity1, EntityT& entity2) noexcept {
	const auto id1 = rawPart(entity1);
	entity1 = Ent::fromRawParts(rawPart(entity2), Version::rawPart(entity1));
	entity2 = Ent::fromRawParts(id1, Version::rawPart(entity2));
}

bool ID::equal(const EntityT entity1, const EntityT entity2) noexcept {
	return rawPart(entity1) == rawPart(entity2);
}

VERSION::Type VERSION::part(const EntityT entity) noexcept {
	return Type(rawPart(entity) >> Id::length);
}

TRAITS::EntityRawT VERSION::asRawPart(const Type version) noexcept {
	return (EntityRawT)version << Id::length;
}

TRAITS::EntityRawT VERSION::rawPart(const EntityT entity) noexcept {
	return Ent::toRaw(entity) & mask;
}

bool VERSION::hasNull(const EntityT entity) noexcept {
	return (Ent::toRaw(entity) & mask) == nullRaw;
}

bool VERSION::hasNotNull(const EntityT entity) noexcept {
	return (Ent::toRaw(entity) & mask) != nullRaw;
}

TRAITS::EntityT VERSION::withNext(const EntityT entity) noexcept {
	return Ent::fromParts(Id::part(entity), next(entity));
}

VERSION::Type VERSION::next(const EntityT entity) noexcept {
	const auto newVersion = part(entity) + 1;

	return newVersion + (newVersion == null);
}

TRAITS::EntityT VERSION::withNull(const EntityT entity) noexcept {
	return Ent::fromRawParts(Id::rawPart(entity), nullRaw);
}

void VERSION::swap(EntityT& entity1, EntityT& entity2) noexcept {
	const auto ver1 = rawPart(entity1);
	entity1 = Ent::fromRawParts(ID::rawPart(entity1), rawPart(entity2));
	entity2 = Ent::fromRawParts(ID::rawPart(entity2), ver1);
}

bool VERSION::equal(const EntityT entity1, const EntityT entity2) noexcept {
	return rawPart(entity1) == rawPart(entity2);
}

} // namespace arch::ecs::_details

#undef TRAITS
#undef ENTITY
#undef ID
#undef VERSION
