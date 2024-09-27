#include <ranges>

#include "CommonComponentPool.h"

namespace arch::ecs::_details { // NOLINT

template<class E>
auto CommonComponentPool<E>::_entitiesForView() const noexcept {
	// entites with version != null are valid
	return std::views::filter(_dense, std::not_fn(_details::EntityTraits<E>::Version::hasNull));
}

} // namespace arch::ecs::_details
