#include "ArchMath.h"
#include "EntityTraits.h"

#define TEMPLATE template<class E>
#define TRAITS EntityTraits<E>
#define ENTITY TRAITS::Entity
#define ID TRAITS::Id
#define VERSION TRAITS::Version

namespace arch::ecs::_details {

TEMPLATE
ENTITY::Type ENTITY::fromParts(IdT id, VersionT ver) noexcept {
	return fromRawParts(Id::asRawPart(id), Version::asRawPart(ver));
}

TEMPLATE
ENTITY::Type ENTITY::fromRawParts(RawT id, RawT ver) noexcept {
	return Type(id | ver);
}

TEMPLATE
ENTITY::Type ENTITY::fromOthers(Type id, Type ver) noexcept {
	return fromRawParts(Id::rawPart(id), Version::rawPart(ver));
}

TEMPLATE
ENTITY::RawT ENTITY::toRaw(Type entity) noexcept {
	return (RawT)entity;
}

TEMPLATE
ID::Type ID::part(EntityT entity) noexcept {
	return { rawPart(entity) };
}

TEMPLATE
TRAITS::EntityRawT ID::asRawPart(Type id) noexcept {
	return (EntityRawT)id & mask;
}

TEMPLATE
TRAITS::EntityRawT ID::rawPart(EntityT entity) noexcept {
	return Entity::toRaw(entity) & mask;
}

TEMPLATE
bool ID::hasNull(EntityT entity) noexcept {
	return (Entity::toRaw(entity) & mask) == nullPart;
}

TEMPLATE
TRAITS::EntityT ID::withNull(EntityT entity) noexcept {
	return Entity::fromRawParts(nullPart, Version::rawPart(entity));
}

TEMPLATE
void ID::swap(EntityT& entity1, EntityT& entity2) noexcept {
	const auto id1 = rawPart(entity1);
	entity1 = Entity::fromRawParts(rawPart(entity2), Version::rawPart(entity1));
	entity2 = Entity::fromRawParts(id1, Version::rawPart(entity2));
}

TEMPLATE
VERSION::Type VERSION::part(EntityT entity) noexcept {
	return Type(rawPart(entity) >> Id::length);
}

TEMPLATE
TRAITS::EntityRawT VERSION::asRawPart(Type version) noexcept {
	return (EntityRawT)version << Id::length;
}

TEMPLATE
TRAITS::EntityRawT VERSION::rawPart(EntityT entity) noexcept {
	return Entity::toRaw(entity) & mask;
}

TEMPLATE
bool VERSION::hasNull(EntityT entity) noexcept {
	return (Entity::toRaw(entity) & mask) == nullPart;
}

TEMPLATE
TRAITS::EntityT VERSION::withNext(EntityT entity) noexcept {
	return Entity::fromParts(Id::part(entity), next(entity));
}

TEMPLATE
VERSION::Type VERSION::next(EntityT entity) noexcept {
	const auto newVersion = part(entity) + 1;

	return newVersion + (newVersion == null);
}

TEMPLATE
TRAITS::EntityT VERSION::withNull(EntityT entity) noexcept {
	return Entity::fromRawParts(Id::rawPart(entity), nullPart);
}

TEMPLATE
void VERSION::swap(EntityT& entity1, EntityT& entity2) noexcept {
	const auto ver1 = rawPart(entity1);
	entity1 = Entity::fromRawParts(ID::rawPart(entity1), rawPart(entity2));
	entity2 = Entity::fromRawParts(ID::rawPart(entity2), ver1);
}

} // namespace arch::ecs::_details

#undef TEMPLATE
#undef TRAITS
#undef ENTITY
#undef ID
#undef VERSION
