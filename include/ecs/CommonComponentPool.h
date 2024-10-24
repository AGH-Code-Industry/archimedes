#pragma once

#include <ranges>

#include "SparseSet.h"
#include <utils/ReadonlyCounter.h>

namespace arch::ecs {

template<bool Const, class I, class Ex>
class View;

}

namespace arch::ecs::_details { // NOLINT

/// @brief Abstract class with behavior shared between all ComponentPools
/// @tparam E - entity type
class CommonComponentPool: public _details::SparseSet, public utils::ReadonlyCounter<size_t> {
public:
	/// @brief Removes component from given entity
	/// @param entity - entity to remove component from
	/// @return If component was actually removed
	virtual bool removeComponent(const Entity entity) noexcept = 0;

protected:
	template<bool, class, class>
	friend class ::arch::ecs::View;

	using EntitiesViewT =
		decltype(std::views::filter(*std::declval<const DenseContainer*>(), _details::EntityTraits::Version::hasNotNull)
		);

	// range with all valid entities in _dense
	EntitiesViewT _entitiesForView() const noexcept;
	static EntitiesViewT _emptyEntitiesForView() noexcept;
};

} // namespace arch::ecs::_details
