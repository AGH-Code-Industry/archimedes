#include <bit>

#include <archimedes/ecs/SparseSet.h>
#include <archimedes/utils/Assert.h>

namespace arch::ecs::_details {

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
