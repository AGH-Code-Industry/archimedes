#include <bit>

#include "SparseSet.h"
#include "utils/Assert.h"

#define TEMPLATE template<class E>
#define SET SparseSet<E>

namespace arch::ecs::_details {

TEMPLATE
typename SET::EntityT* SET::_sparseAssurePage(const size_t n) noexcept {
	// resize(n) makes capacity == n (bad)
	if (_sparse.size() < n + 1) {
		_sparse.reserve(std::bit_ceil(n + 1));
		_sparse.resize(n + 1);
	}

	auto& page = _sparse[n];
	if (page == nullptr) {
		page = std::make_unique<std::array<EntityT, Traits::pageSize>>();
		page->fill(Traits::Entity::null);
	}

	return page->data();
}

TEMPLATE
typename SET::EntityT& SET::_sparseAssure(const IdT id) noexcept {
	return _sparseAssurePage(id / Traits::pageSize)[id % Traits::pageSize];
}

TEMPLATE
typename SET::EntityT& SET::_sparseGet(const IdT id) noexcept {
	ARCH_ASSERT(
		id / Traits::pageSize < _sparse.size() && _sparse[id / Traits::pageSize] != nullptr,
		"Page for given id does not exist"
	);
	return (*_sparse[id / Traits::pageSize])[id % Traits::pageSize];
}

TEMPLATE
const typename SET::EntityT& SET::_sparseGet(const IdT id) const noexcept {
	return const_cast<SET*>(this)->_sparseGet(id);
}

TEMPLATE
typename SET::EntityT* SET::_sparseTryGet(const IdT id) noexcept {
	const size_t pageNum = id / Traits::pageSize;

	if (_sparse.size() <= pageNum) {
		return nullptr;
	}

	auto page = _sparse[pageNum].get();

	return page ? page->data() + id % Traits::pageSize : nullptr;
}

TEMPLATE
const typename SET::EntityT* SET::_sparseTryGet(const IdT id) const noexcept {
	return const_cast<SET*>(this)->_sparseTryGet(id);
}

TEMPLATE
size_t SET::find(const IdT id) const noexcept {
	const auto sparsePtr = _sparseTryGet(id);

	return sparsePtr && !Traits::Version::hasNull(*sparsePtr) ? Traits::Id::part(*sparsePtr) : (size_t)-1;
}

TEMPLATE
size_t SET::find(const EntityT entity) const noexcept {
	const auto sparsePtr = _sparseTryGet(Traits::Id::part(entity));

	return sparsePtr && Traits::Version::equal(*sparsePtr, entity) ? Traits::Id::part(*sparsePtr) : (size_t)-1;
}

TEMPLATE
typename SET::VersionT SET::version(const IdT id) const noexcept {
	const auto sparsePtr = _sparseTryGet(id);

	return sparsePtr ? Traits::Version::part(*sparsePtr) : Traits::Version::null;
}

TEMPLATE
typename SET::VersionT SET::version(const EntityT entity) const noexcept {
	return version(Traits::Id::part(entity));
}

TEMPLATE
bool SET::contains(const IdT id) const noexcept {
	const auto sparsePtr = _sparseTryGet(id);

	return sparsePtr && !Traits::Version::hasNull(*sparsePtr);
}

TEMPLATE
bool SET::contains(const EntityT entity) const noexcept {
	const auto sparsePtr = _sparseTryGet(Traits::Id::part(entity));

	return sparsePtr && Traits::Version::equal(*sparsePtr, entity);
}

} // namespace arch::ecs::_details

#undef TEMPLATE
#undef SET
