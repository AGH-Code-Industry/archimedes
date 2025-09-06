#include <ranges>

#include <archimedes/ecs/CommonComponentPool.h>

namespace arch::ecs::_details { // NOLINT

CommonComponentPool::EntitiesViewT CommonComponentPool::_entitiesForView() const noexcept {
	// entites with version != null are valid
	return std::views::filter(_dense, _details::EntityTraits::Version::hasNotNull);
}

CommonComponentPool::EntitiesViewT CommonComponentPool::_emptyEntitiesForView() noexcept {
	static const std::vector<Entity> empty;
	return std::views::filter(empty, _details::EntityTraits::Version::hasNotNull);
}

} // namespace arch::ecs::_details
