#pragma once

#include "utils/Assert.h"
#include <ecs/EntityPool.h>

#define POOL EntityPool

// https://miro.com/app/board/uXjVK4gF1DI=/?share_link_id=296698570044
// ^ picture explanations

namespace arch::ecs {

POOL::Iterator POOL::begin() noexcept {
	return _dense.begin();
}

POOL::ConstIterator POOL::begin() const noexcept {
	return _dense.begin();
}

POOL::ConstIterator POOL::cbegin() const noexcept {
	return _dense.cbegin();
}

POOL::Iterator POOL::end() noexcept {
	return _dense.begin() + _size;
}

POOL::ConstIterator POOL::end() const noexcept {
	return _dense.begin() + _size;
}

POOL::ConstIterator POOL::cend() const noexcept {
	return _dense.cbegin() + _size;
}

POOL::ReverseIterator POOL::rbegin() noexcept {
	return std::make_reverse_iterator(end());
}

POOL::ConstReverseIterator POOL::rbegin() const noexcept {
	return std::make_reverse_iterator(end());
}

POOL::ConstReverseIterator POOL::crbegin() const noexcept {
	return std::make_reverse_iterator(cend());
}

POOL::ReverseIterator POOL::rend() noexcept {
	return std::make_reverse_iterator(begin());
}

POOL::ConstReverseIterator POOL::rend() const noexcept {
	return std::make_reverse_iterator(begin());
}

POOL::ConstReverseIterator POOL::crend() const noexcept {
	return std::make_reverse_iterator(cbegin());
}

void POOL::swap(POOL& other) noexcept {
	std::swap(_sparse, other._sparse);
	std::swap(_dense, other._dense);
	std::swap(_size, other._size);
}

POOL::EntityT POOL::newEntity() noexcept {
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
		_sparseGet(Traits::Id::part(entity)) = entity;

		return entity;
	}
}

POOL::EntityT POOL::recycleEntity(const EntityT entity) noexcept {
	if (!contains(Traits::Id::part(entity)) /*&& _size <= Traits::Id::max*/) {
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

POOL::EntityT POOL::recycleId(const IdT id) noexcept {
	if (!contains(id)) {
		auto& wantedSparse = _sparseGet(id);
		auto& toSwapDense = _dense[_size++];

		std::swap(_dense[Traits::Id::part(wantedSparse)], toSwapDense);
		Traits::Id::swap(wantedSparse, _sparseGet(Traits::Id::part(toSwapDense)));

		wantedSparse = Traits::Ent::fromOthers(wantedSparse, toSwapDense);

		return toSwapDense;
	}
	return null;
}

void POOL::kill(const EntityT entity) noexcept {
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

void POOL::kill(const IdT id) noexcept {
	if (contains(id)) {
		auto& wantedSparse = _sparseGet(id);
		auto& toSwapDense = _dense[--_size];

		const auto temp = toSwapDense;
		std::swap(_dense[Traits::Id::part(wantedSparse)], toSwapDense);
		Traits::Id::swap(wantedSparse, _sparseGet(Traits::Id::part(temp)));

		toSwapDense = Traits::Version::withNext(toSwapDense);
		wantedSparse = Traits::Version::withNull(wantedSparse);
	}
}

void POOL::kill(std::input_iterator auto first, std::input_iterator auto last) noexcept {
	for (; first != last; ++first) {
		kill(*first);
	}
}

void POOL::kill(std::initializer_list<EntityT> entities) noexcept {
	return kill(entities.begin(), entities.end());
}

void POOL::kill(std::initializer_list<IdT> ids) noexcept {
	return kill(ids.begin(), ids.end());
}

POOL::Iterator POOL::find(const EntityT entity) noexcept {
	return find(Traits::Id::part(entity));
}

POOL::ConstIterator POOL::find(const EntityT entity) const noexcept {
	return find(Traits::Id::part(entity));
}

POOL::Iterator POOL::find(const IdT id) noexcept {
	if (id == Traits::Id::null) {
		return end();
	}

	const size_t _id = id;
	const size_t pageNum = _id / Traits::pageSize;

	return pageNum < _sparse.size() && _sparse[pageNum] != nullptr &&
			!Traits::Version::hasNull((*_sparse[pageNum])[_id % Traits::pageSize]) ?
		begin() + Traits::Version::rawPart((*_sparse[pageNum])[_id % Traits::pageSize]) :
		end();
}

POOL::ConstIterator POOL::find(const IdT id) const noexcept {
	if (id == Traits::Id::null) {
		return end();
	}

	const size_t _id = id;
	const size_t pageNum = _id / Traits::pageSize;

	return pageNum < _sparse.size() && _sparse[pageNum] != nullptr &&
			!Traits::Version::hasNull((*_sparse[pageNum])[_id % Traits::pageSize]) ?
		begin() + Traits::Version::rawPart((*_sparse[pageNum])[_id % Traits::pageSize]) :
		end();
}

size_t POOL::size() const noexcept {
	return _size;
}

auto POOL::_debug() noexcept -> std::tuple<typename Base::SparseContainer*, typename Base::DenseContainer*> {
	return { &_sparse, &_dense };
}

} // namespace arch::ecs

#undef POOL
