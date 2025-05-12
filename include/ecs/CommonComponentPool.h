#pragma once

#include <ranges>

#include "SparseSet.h"
#include <utils/ReadonlyCounter.h>

namespace arch::ecs::_details { // NOLINT

/// @brief Abstract class with behavior shared between all ComponentPools
class CommonComponentPool: public _details::SparseSet, public utils::ReadonlyCounter<size_t> {
public:
	/// @brief Removes component from given entity
	/// @param entity - entity to remove component from
	/// @return If component was actually removed
	virtual bool removeComponent(const Entity entity) noexcept = 0;

private:

	template<class, class>
	friend class View;
	template<class, class>
	friend class ViewIterator;
};

} // namespace arch::ecs::_details
