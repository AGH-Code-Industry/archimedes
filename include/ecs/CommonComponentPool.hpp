#include <ranges>

#include "CommonComponentPool.h"

namespace arch::ecs::_details { // NOLINT

template<class E>
auto CommonComponentPool<E>::_entitiesForView() const noexcept {
	// entites with version != null are valid
	return std::views::filter(_dense, _details::EntityTraits<E>::Version::hasNotNull);
}

template<class E>
auto CommonComponentPool<E>::_emptyEntitiesForView() noexcept {
	static const std::vector<E> empty;
	return std::views::filter(empty, _details::EntityTraits<E>::Version::hasNotNull);
}

} // namespace arch::ecs::_details
