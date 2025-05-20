#include "utils/Assert.h"
#include <ecs/EntityPool.h>

// https://miro.com/app/board/uXjVK4gF1DI=/?share_link_id=296698570044
// ^ picture explanations

namespace arch::ecs {

EntityPool::EntityT EntityPool::newEntity() noexcept {
	if (_size == Traits::Id::max + 1) { // entity limit achieved
		return null;
	}
	if (_size == _dense.size()) { // new entity
		const auto entity = Traits::Ent::fromParts(_size++, 0);

		_dense.push_back(entity);
		_sparseAssure(Traits::Id::part(entity)) = entity;

		return entity;
	} else { // recycle
		const auto entity = _dense[_size++];
		auto&& inSparse = _sparseGet(Traits::Id::part(entity));

		inSparse = Traits::Ent::fromOthers(inSparse, entity);

		return entity;
	}
}

EntityPool::EntityT EntityPool::recycleEntity(const EntityT entity) noexcept {
	if (!containsID(Traits::Id::part(entity)) /*&& _size <= Traits::Id::max*/) {
		auto& wantedSparse = _sparseGet(Traits::Id::part(entity));
		auto& toSwapDense = _dense[_size++];

		// std::swap(_dense[Traits::Id::part(wantedSparse)], toSwapDense);
		Traits::Id::swap(wantedSparse, _sparseGet(Traits::Id::part(toSwapDense)));

		toSwapDense = Traits::Ent::fromOthers(toSwapDense, entity);
		wantedSparse = Traits::Ent::fromOthers(wantedSparse, toSwapDense);

		return toSwapDense;
	}
	return null;
}

EntityPool::EntityT EntityPool::recycleId(const IdT id) noexcept {
	if (!containsID(id)) {
		auto& wantedSparse = _sparseGet(id);
		auto& toSwapDense = _dense[_size++];

		std::swap(_dense[Traits::Id::part(wantedSparse)], toSwapDense);
		Traits::Id::swap(wantedSparse, _sparseGet(Traits::Id::part(toSwapDense)));

		wantedSparse = Traits::Ent::fromOthers(wantedSparse, toSwapDense);

		return toSwapDense;
	}
	return null;
}

void EntityPool::kill(const EntityT entity) noexcept {
	if (contains(entity)) {
		auto& wantedSparse = _sparseGet(Traits::Id::part(entity));
		auto& toSwapDense = _dense[--_size];

		const auto temp = toSwapDense;
		std::swap(_dense[Traits::Id::part(wantedSparse)], toSwapDense);
		Traits::Id::swap(wantedSparse, _sparseGet(Traits::Id::part(temp)));

		toSwapDense = Traits::Version::withNext(toSwapDense);
		wantedSparse = Traits::Version::withNull(wantedSparse);
	}
}

void EntityPool::kill(const IdT id) noexcept {
	if (containsID(id)) {
		auto& wantedSparse = _sparseGet(id);
		auto& toSwapDense = _dense[--_size];

		const auto temp = toSwapDense;
		std::swap(_dense[Traits::Id::part(wantedSparse)], toSwapDense);
		Traits::Id::swap(wantedSparse, _sparseGet(Traits::Id::part(temp)));

		toSwapDense = Traits::Version::withNext(toSwapDense);
		wantedSparse = Traits::Version::withNull(wantedSparse);
	}
}

void EntityPool::kill(std::input_iterator auto first, std::input_iterator auto last) noexcept {
	for (; first != last; ++first) {
		kill(*first);
	}
}

void EntityPool::kill(std::initializer_list<EntityT> entities) noexcept {
	return kill(entities.begin(), entities.end());
}

void EntityPool::kill(std::initializer_list<IdT> ids) noexcept {
	return kill(ids.begin(), ids.end());
}

EntityPool::Iterator EntityPool::find(const EntityT entity) noexcept {
	return find(Traits::Id::part(entity));
}

EntityPool::ConstIterator EntityPool::find(const EntityT entity) const noexcept {
	return find(Traits::Id::part(entity));
}

EntityPool::Iterator EntityPool::find(const IdT id) noexcept {
	if (id == Traits::Id::null) {
		return end();
	}

	const size_t _id = id;
	const size_t pageNum = _id / Traits::pageSize;

	return pageNum < _sparse.size() && _sparse[pageNum] != nullptr &&
			!Traits::Version::hasNull(_sparse[pageNum][_id % Traits::pageSize]) ?
		begin() + Traits::Version::rawPart(_sparse[pageNum][_id % Traits::pageSize]) :
		end();
}

EntityPool::ConstIterator EntityPool::find(const IdT id) const noexcept {
	if (id == Traits::Id::null) {
		return end();
	}

	const size_t _id = id;
	const size_t pageNum = _id / Traits::pageSize;

	return pageNum < _sparse.size() && _sparse[pageNum] != nullptr &&
			!Traits::Version::hasNull(_sparse[pageNum][_id % Traits::pageSize]) ?
		begin() + Traits::Version::rawPart(_sparse[pageNum][_id % Traits::pageSize]) :
		end();
}

size_t EntityPool::size() const noexcept {
	return _size;
}

auto EntityPool::_debug() noexcept -> std::tuple<typename Base::SparseContainer*, typename Base::DenseContainer*> {
	return { &_sparse, &_dense };
}

} // namespace arch::ecs
