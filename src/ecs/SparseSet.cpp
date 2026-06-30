#include <bit>

#include <archimedes/ecs/SparseSet.h>
#include <archimedes/utils/Assert.h>

namespace arch::ecs::_details {

SparseSet::SparseSet(const SparseSet& other) noexcept {
	*this = other;
}

SparseSet& SparseSet::operator=(const SparseSet& other) noexcept {
	_dense = other._dense;

	for (auto i = other._sparse.size() - 1; i != (size_t)-1; --i) {
		// copy memory layout
		if (other._sparse[i] != nullptr) {
			std::copy(other._sparse[i].get(), other._sparse[i].get() + Traits::pageSize, _sparseAssurePage(i));
		}
	}

	return *this;
}

bool SparseSet::operator==(const SparseSet& other) const noexcept {
	if (count() != other.count()) {
		return false;
	}

	for (auto&& entity : _dense) {
		// set-wise comparision
		if (!other.contains(entity)) {
			return false;
		}
	}

	return true;
}

typename SparseSet::EntityT* SparseSet::_sparseAssurePage(const size_t n) noexcept {
	// resize(n) only would make capacity == n (bad)
	if (_sparse.size() < n + 1) {
		_sparse.reserve(std::bit_ceil(n + 1));
		_sparse.resize(n + 1);
	}

	auto& page = _sparse[n];
	if (page == nullptr) {
		page = std::unique_ptr<EntityT[]>(new (std::nothrow) EntityT[Traits::pageSize]);
		const auto end = page.get() + Traits::pageSize;
		for (auto val = page.get(); val != end; ++val) {
			*val = Traits::Ent::null;
		}
	}

	return page.get();
}

} // namespace arch::ecs::_details
