#include "ArchMath.h"
#include "EntityTraits.h"

#define TEMPLATE template<class E>
#define TRAITS EntityTraits<E>
#define ENTITY TRAITS::Entity
#define ID TRAITS::Id
#define VERSION TRAITS::Version

namespace arch::ecs::_details {

TEMPLATE
ENTITY::Type ENTITY::fromParts(const IdT id, const VersionT ver) noexcept {
	return fromRawParts(Id::asRawPart(id), Version::asRawPart(ver));
}

TEMPLATE
ENTITY::Type ENTITY::fromRawParts(const RawT id, const RawT ver) noexcept {
	return Type(id | ver);
}

TEMPLATE
ENTITY::Type ENTITY::fromOthers(const Type id, const Type ver) noexcept {
	return fromRawParts(Id::rawPart(id), Version::rawPart(ver));
}

TEMPLATE
ENTITY::RawT ENTITY::toRaw(const Type entity) noexcept {
	return (RawT)entity;
}

TEMPLATE
ID::Type ID::part(const EntityT entity) noexcept {
	return { rawPart(entity) };
}

TEMPLATE
TRAITS::EntityRawT ID::asRawPart(const Type id) noexcept {
	return (EntityRawT)id & mask;
}

TEMPLATE
TRAITS::EntityRawT ID::rawPart(const EntityT entity) noexcept {
	return Entity::toRaw(entity) & mask;
}

TEMPLATE
bool ID::hasNull(const EntityT entity) noexcept {
	return (Entity::toRaw(entity) & mask) == nullRaw;
}

TEMPLATE
TRAITS::EntityT ID::withNull(const EntityT entity) noexcept {
	return Entity::fromRawParts(nullRaw, Version::rawPart(entity));
}

TEMPLATE
void ID::swap(EntityT& entity1, EntityT& entity2) noexcept {
	const auto id1 = rawPart(entity1);
	entity1 = Entity::fromRawParts(rawPart(entity2), Version::rawPart(entity1));
	entity2 = Entity::fromRawParts(id1, Version::rawPart(entity2));
}

TEMPLATE
bool ID::equal(const EntityT entity1, const EntityT entity2) noexcept {
	return rawPart(entity1) == rawPart(entity2);
}

TEMPLATE
VERSION::Type VERSION::part(const EntityT entity) noexcept {
	return Type(rawPart(entity) >> Id::length);
}

TEMPLATE
TRAITS::EntityRawT VERSION::asRawPart(const Type version) noexcept {
	return (EntityRawT)version << Id::length;
}

TEMPLATE
TRAITS::EntityRawT VERSION::rawPart(const EntityT entity) noexcept {
	return Entity::toRaw(entity) & mask;
}

TEMPLATE
bool VERSION::hasNull(const EntityT entity) noexcept {
	return (Entity::toRaw(entity) & mask) == nullRaw;
}

TEMPLATE
TRAITS::EntityT VERSION::withNext(const EntityT entity) noexcept {
	return Entity::fromParts(Id::part(entity), next(entity));
}

TEMPLATE
VERSION::Type VERSION::next(const EntityT entity) noexcept {
	const auto newVersion = part(entity) + 1;

	return newVersion + (newVersion == null);
}

TEMPLATE
TRAITS::EntityT VERSION::withNull(const EntityT entity) noexcept {
	return Entity::fromRawParts(Id::rawPart(entity), nullRaw);
}

TEMPLATE
void VERSION::swap(EntityT& entity1, EntityT& entity2) noexcept {
	const auto ver1 = rawPart(entity1);
	entity1 = Entity::fromRawParts(ID::rawPart(entity1), rawPart(entity2));
	entity2 = Entity::fromRawParts(ID::rawPart(entity2), ver1);
}

TEMPLATE
bool VERSION::equal(const EntityT entity1, const EntityT entity2) noexcept {
	return rawPart(entity1) == rawPart(entity2);
}

} // namespace arch::ecs::_details

#undef TEMPLATE
#undef TRAITS
#undef ENTITY
#undef ID
#undef VERSION
