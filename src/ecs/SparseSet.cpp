#include <bit>

#include "utils/Assert.h"
#include <ecs/SparseSet.h>

namespace arch::ecs::_details {

typename SparseSet::EntityT* SparseSet::_sparseAssurePage(const size_t n) noexcept {
	// resize(n) only would make capacity == n (bad)
	if (_sparse.size() < n + 1) {
		_sparse.reserve(std::bit_ceil(n + 1));
		_sparse.resize(n + 1);
	}

	auto& page = _sparse[n];
	if (page == nullptr) {
		page = std::make_unique<std::array<EntityT, Traits::pageSize>>();
		page->fill(Traits::Ent::null);
	}

	return page->data();
}

typename SparseSet::EntityT& SparseSet::_sparseAssure(const IdT id) noexcept {
	return _sparseAssurePage(id / Traits::pageSize)[id % Traits::pageSize];
}

typename SparseSet::EntityT& SparseSet::_sparseGet(const IdT id) noexcept {
	ARCH_ASSERT(
		id / Traits::pageSize < _sparse.size() && _sparse[id / Traits::pageSize] != nullptr,
		"Page for given id does not exist"
	);
	return (*_sparse[id / Traits::pageSize])[id % Traits::pageSize];
}

const typename SparseSet::EntityT& SparseSet::_sparseGet(const IdT id) const noexcept {
	return const_cast<SparseSet*>(this)->_sparseGet(id);
}

typename SparseSet::EntityT* SparseSet::_sparseTryGet(const IdT id) noexcept {
	const size_t pageNum = id / Traits::pageSize;

	if (_sparse.size() <= pageNum) {
		return nullptr;
	}

	auto page = _sparse[pageNum].get();

	return page ? page->data() + id % Traits::pageSize : nullptr;
}

const typename SparseSet::EntityT* SparseSet::_sparseTryGet(const IdT id) const noexcept {
	return const_cast<SparseSet*>(this)->_sparseTryGet(id);
}

size_t SparseSet::find(const IdT id) const noexcept {
	const auto sparsePtr = _sparseTryGet(id);

	return sparsePtr && !Traits::Version::hasNull(*sparsePtr) ? Traits::Id::part(*sparsePtr) : (size_t)-1;
}

size_t SparseSet::find(const EntityT entity) const noexcept {
	const auto sparsePtr = _sparseTryGet(Traits::Id::part(entity));

	return sparsePtr && Traits::Version::equal(*sparsePtr, entity) ? Traits::Id::part(*sparsePtr) : (size_t)-1;
}

typename SparseSet::VersionT SparseSet::version(const IdT id) const noexcept {
	const auto sparsePtr = _sparseTryGet(id);

	return sparsePtr ? Traits::Version::part(*sparsePtr) : Traits::Version::null;
}

typename SparseSet::VersionT SparseSet::version(const EntityT entity) const noexcept {
	return version(Traits::Id::part(entity));
}

bool SparseSet::contains(const IdT id) const noexcept {
	const auto sparsePtr = _sparseTryGet(id);

	return sparsePtr && !Traits::Version::hasNull(*sparsePtr);
}

bool SparseSet::contains(const EntityT entity) const noexcept {
	const auto sparsePtr = _sparseTryGet(Traits::Id::part(entity));

	return sparsePtr && Traits::Version::equal(*sparsePtr, entity);
}

} // namespace arch::ecs::_details
