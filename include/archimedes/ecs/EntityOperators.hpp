#include "EntityOperators.h"

namespace arch::ecs {

constexpr _details::EntityTraits::IdT operator*(Entity entity) noexcept {
	return _details::EntityTraits::Id::part(entity);
}

constexpr _details::EntityTraits::VersionT operator~(Entity entity) noexcept {
	return _details::EntityTraits::Version::part(entity);
}

} // namespace arch::ecs
