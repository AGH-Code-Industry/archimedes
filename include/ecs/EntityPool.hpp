#pragma once

#include "EntityPool.h"
#include "utils/Assert.h"

#define TEMPLATE_E template<class E>
#define POOL_E EntityPool<E>

// https://miro.com/app/board/uXjVK4gF1DI=/?share_link_id=296698570044
// ^ picture explanations

namespace arch::ecs {

TEMPLATE_E POOL_E::Iterator POOL_E::begin() noexcept {
	return _dense.begin();
}

TEMPLATE_E
POOL_E::ConstIterator POOL_E::begin() const noexcept {
	return _dense.begin();
}

TEMPLATE_E
POOL_E::ConstIterator POOL_E::cbegin() const noexcept {
	return _dense.cbegin();
}

TEMPLATE_E
POOL_E::Iterator POOL_E::end() noexcept {
	return _dense.begin() + _size;
}

TEMPLATE_E
POOL_E::ConstIterator POOL_E::end() const noexcept {
	return _dense.begin() + _size;
}

TEMPLATE_E
POOL_E::ConstIterator POOL_E::cend() const noexcept {
	return _dense.cbegin() + _size;
}

TEMPLATE_E
POOL_E::ReverseIterator POOL_E::rbegin() noexcept {
	return std::make_reverse_iterator(end());
}

TEMPLATE_E
POOL_E::ConstReverseIterator POOL_E::rbegin() const noexcept {
	return std::make_reverse_iterator(end());
}

TEMPLATE_E
POOL_E::ConstReverseIterator POOL_E::crbegin() const noexcept {
	return std::make_reverse_iterator(cend());
}

TEMPLATE_E
POOL_E::ReverseIterator POOL_E::rend() noexcept {
	return std::make_reverse_iterator(begin());
}

TEMPLATE_E
POOL_E::ConstReverseIterator POOL_E::rend() const noexcept {
	return std::make_reverse_iterator(begin());
}

TEMPLATE_E
POOL_E::ConstReverseIterator POOL_E::crend() const noexcept {
	return std::make_reverse_iterator(cbegin());
}

TEMPLATE_E
void POOL_E::swap(POOL_E& other) noexcept {
	std::swap(_sparse, other._sparse);
	std::swap(_dense, other._dense);
	std::swap(_size, other._size);
}

TEMPLATE_E
POOL_E::EntityT POOL_E::newEntity() noexcept {
	if (_size == Traits::Id::max + 1) { // entity limit achieved
		return null;
	}
	if (_size == _dense.size()) { // new entity
		const auto entity = Traits::Entity::fromParts(_size++, 0);

		_dense.push_back(entity);
		_sparseAssure(Traits::Id::part(entity)) = entity;

		return entity;
	} else { // recycle
		const auto entity = _dense[_size++];
		_sparseGet(Traits::Id::part(entity)) = entity;

		return entity;
	}
}

TEMPLATE_E
POOL_E::EntityT POOL_E::recycleEntity(const EntityT entity) noexcept {
	if (!contains(Traits::Id::part(entity)) /*&& _size <= Traits::Id::max*/) {
		auto& wantedSparse = _sparseGet(entity);
		auto& toSwapDense = _dense[_size++];

		std::swap(_dense[Traits::Id::part(wantedSparse)], toSwapDense);
		Traits::Id::swap(wantedSparse, _sparseGet(toSwapDense));

		toSwapDense = Traits::Entity::fromOthers(toSwapDense, entity);
		wantedSparse = Traits::Entity::fromOthers(wantedSparse, toSwapDense);

		return toSwapDense;
	}
	return null;
}

TEMPLATE_E
POOL_E::EntityT POOL_E::recycleId(const IdT id) noexcept {
	if (!contains(id)) {
		auto& wantedSparse = _sparseGet(id);
		auto& toSwapDense = _dense[_size++];

		std::swap(_dense[Traits::Id::part(wantedSparse)], toSwapDense);
		Traits::Id::swap(wantedSparse, _sparseGet(toSwapDense));

		wantedSparse = Traits::Entity::fromOthers(wantedSparse, toSwapDense);

		return toSwapDense;
	}
	return null;
}

TEMPLATE_E
void POOL_E::kill(const EntityT entity) noexcept {
	if (contains(entity)) {
		auto& wantedSparse = _sparseGet(Traits::Id::part(entity));
		auto& toSwapDense = _dense[--_size];

		std::swap(_dense[Traits::Id::part(wantedSparse)], toSwapDense);
		Traits::Id::swap(wantedSparse, _sparseGet(Traits::Id::part(toSwapDense)));

		toSwapDense = Traits::Version::withNext(toSwapDense);
		wantedSparse = Traits::Version::withNull(wantedSparse);
	}
}

TEMPLATE_E
void POOL_E::kill(const IdT id) noexcept {
	if (contains(id)) {
		auto& wantedSparse = _sparseGet(id);
		auto& toSwapDense = _dense[--_size];

		std::swap(_dense[Traits::Id::part(wantedSparse)], toSwapDense);
		Traits::Id::swap(wantedSparse, _sparseGet(toSwapDense));

		toSwapDense = Traits::Version::withNext(toSwapDense);
		wantedSparse = Traits::Version::withNull(wantedSparse);
	}
}

TEMPLATE_E
void POOL_E::kill(std::input_iterator auto first, std::input_iterator auto last) noexcept {
	for (; first != last; ++first) {
		kill(*first);
	}
}

TEMPLATE_E
void POOL_E::kill(std::initializer_list<EntityT> entities) noexcept {
	return kill(entities.begin(), entities.end());
}

TEMPLATE_E
void POOL_E::kill(std::initializer_list<IdT> ids) noexcept {
	return kill(ids.begin(), ids.end());
}

TEMPLATE_E
POOL_E::Iterator POOL_E::find(const EntityT entity) noexcept {
	return find(Traits::Id::part(entity));
}

TEMPLATE_E
POOL_E::ConstIterator POOL_E::find(const EntityT entity) const noexcept {
	return find(Traits::Id::part(entity));
}

TEMPLATE_E
POOL_E::Iterator POOL_E::find(const IdT id) noexcept {
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

TEMPLATE_E
POOL_E::ConstIterator POOL_E::find(const IdT id) const noexcept {
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

TEMPLATE_E
size_t POOL_E::size() const noexcept {
	return _size;
}

} // namespace arch::ecs

#undef TEMPLATE_E
#undef POOL_E
