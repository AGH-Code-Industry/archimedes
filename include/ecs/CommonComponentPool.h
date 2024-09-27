#pragma once

#include "SparseSet.h"
#include <utils/ReadonlyCounter.h>

namespace arch::ecs {

template<class E, bool Const, class I, class Ex>
class View;

}

namespace arch::ecs::_details { // NOLINT

/// @brief Abstract class with behavior shared between all ComponentPools
/// @tparam E - entity type
template<class E>
class CommonComponentPool: public _details::SparseSet<E>, public utils::ReadonlyCounter<size_t> {
public:
	/// @brief Removes component from given entity
	/// @param entity - entity to remove component from
	/// @return If component was actually removed
	virtual bool removeComponent(const E entity) noexcept = 0;

	using _details::SparseSet<E>::contains;

	using utils::ReadonlyCounter<size_t>::count;

protected:
	template<class E, bool Const, class I, class Ex>
	friend class ::arch::ecs::View;

	using _details::SparseSet<E>::_sparseAssure;
	using _details::SparseSet<E>::_sparseAssurePage;
	using _details::SparseSet<E>::_sparseGet;
	using _details::SparseSet<E>::_sparseTryGet;

	using utils::ReadonlyCounter<size_t>::_counter;
	using _details::SparseSet<E>::_dense;
	using _details::SparseSet<E>::_sparse;

	// range with all valid entities in _dense
	auto _entitiesForView() const noexcept;
};

} // namespace arch::ecs::_details

#include "CommonComponentPool.hpp"
