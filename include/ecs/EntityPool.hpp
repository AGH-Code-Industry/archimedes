#pragma once

#include "EntityPool.h"

#define TEMPLATE template<class E>
#define POOL EntityPool<E>

namespace arch::ecs {

TEMPLATE
typename POOL::EntityT* POOL::_tryInitPage(const size_t n) noexcept {
	if (_sparse.size() <= n) {
		while (_sparse.size() != n + 1) {
			_sparse.emplace_back();
		}
		//_sparse.resize(n + 1);
	}

	auto& page = _sparse[n];
	if (page == nullptr) {
		page = std::make_unique<EntityT[]>(Traits::pageSize);
		std::fill(page.get(), page.get() + Traits::pageSize, null);
	}

	return page.get();
}

TEMPLATE
typename POOL::EntityT& POOL::_sparseAssure(const EntityT entity) noexcept {
	return _sparseAssure(Traits::Id::part(entity));
}

TEMPLATE
typename POOL::EntityT& POOL::_sparseAssure(const IdT id) noexcept {
	return _tryInitPage(qdiv<Traits::pageSize>(id))[qmod<Traits::pageSize>(id)];
}

TEMPLATE
typename POOL::EntityT& POOL::_sparseGet(const EntityT entity) noexcept {
	return _sparseGet(Traits::Id::part(entity));
}

TEMPLATE
const typename POOL::EntityT& POOL::_sparseGet(const EntityT entity) const noexcept {
	return _sparseGet(Traits::Id::part(entity));
}

TEMPLATE
typename POOL::EntityT& POOL::_sparseGet(const IdT id) noexcept {
	return _sparse[qdiv<Traits::pageSize>(id)][qmod<Traits::pageSize>(id)];
}

TEMPLATE
const typename POOL::EntityT& POOL::_sparseGet(const IdT id) const noexcept {
	return _sparse[qdiv<Traits::pageSize>(id)][qmod<Traits::pageSize>(id)];
}

TEMPLATE POOL::Iterator POOL::begin() noexcept {
	return _dense.begin();
}

TEMPLATE
POOL::ConstIterator POOL::begin() const noexcept {
	return _dense.begin();
}

TEMPLATE
POOL::ConstIterator POOL::cbegin() const noexcept {
	return _dense.cbegin();
}

TEMPLATE
POOL::Iterator POOL::end() noexcept {
	return _dense.begin() + _size;
}

TEMPLATE
POOL::ConstIterator POOL::end() const noexcept {
	return _dense.begin() + _size;
}

TEMPLATE
POOL::ConstIterator POOL::cend() const noexcept {
	return _dense.cbegin() + _size;
}

TEMPLATE
POOL::ReverseIterator POOL::rbegin() noexcept {
	return std::make_reverse_iterator(end());
}

TEMPLATE
POOL::ConstReverseIterator POOL::rbegin() const noexcept {
	return std::make_reverse_iterator(end());
}

TEMPLATE
POOL::ConstReverseIterator POOL::crbegin() const noexcept {
	return std::make_reverse_iterator(cend());
}

TEMPLATE
POOL::ReverseIterator POOL::rend() noexcept {
	return std::make_reverse_iterator(begin());
}

TEMPLATE
POOL::ConstReverseIterator POOL::rend() const noexcept {
	return std::make_reverse_iterator(begin());
}

TEMPLATE
POOL::ConstReverseIterator POOL::crend() const noexcept {
	return std::make_reverse_iterator(cbegin());
}

TEMPLATE
void POOL::swap(POOL& other) noexcept {
	std::swap(_sparse, other._sparse);
	std::swap(_dense, other._dense);
	std::swap(_size, other._size);
}

TEMPLATE
POOL::EntityT POOL::newEntity() noexcept {
	if (_size == ((1 << Traits::Id::length) - 1)) { // entity limit achieved
		return null;
	}
	if (_size == _dense.size()) { // new entity
		const auto entity = Traits::Entity::fromParts(_size++, 0);

		_dense.push_back(entity);
		_sparseAssure(entity) = entity;

		return entity;
	} else { // recycle
		const auto entity = _dense[_size++];
		_sparseGet(entity) = entity;

		return entity;
	}
}

TEMPLATE
POOL::EntityT POOL::recycleEntity(const EntityT entity) noexcept {
	if (not contains(Traits::Id::part(entity)) /*and _size <= Traits::Id::max*/) {
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

TEMPLATE
POOL::EntityT POOL::recycleId(const IdT id) noexcept {
	if (not contains(id)) {
		auto& wantedSparse = _sparseGet(id);
		auto& toSwapDense = _dense[_size++];

		std::swap(_dense[Traits::Id::part(wantedSparse)], toSwapDense);
		Traits::Id::swap(wantedSparse, _sparseGet(toSwapDense));

		wantedSparse = Traits::Entity::fromOthers(wantedSparse, toSwapDense);

		return toSwapDense;
	}
	return null;
}

TEMPLATE
void POOL::kill(const EntityT entity) noexcept {
	if (alive(entity)) {
		auto& wantedSparse = _sparseGet(entity);
		auto& toSwapDense = _dense[--_size];

		std::swap(_dense[Traits::Id::part(wantedSparse)], toSwapDense);
		Traits::Id::swap(wantedSparse, _sparseGet(toSwapDense));

		toSwapDense = Traits::Version::withNext(toSwapDense);
		wantedSparse = Traits::Version::withNull(wantedSparse);
	}
}

TEMPLATE
void POOL::kill(const IdT id) noexcept {
	if (contains(id)) {
		auto& wantedSparse = _sparseGet(id);
		auto& toSwapDense = _dense[--_size];

		std::swap(_dense[Traits::Id::part(wantedSparse)], toSwapDense);
		Traits::Id::swap(wantedSparse, _sparseGet(toSwapDense));

		toSwapDense = Traits::Version::withNext(toSwapDense);
		wantedSparse = Traits::Version::withNull(wantedSparse);
	}
}

TEMPLATE
void POOL::kill(std::input_iterator auto first, std::input_iterator auto last) noexcept {
	for (; first != last; ++first) {
		kill(*first);
	}
}

TEMPLATE
void POOL::kill(std::initializer_list<EntityT> entities) noexcept {
	return kill(entities.begin(), entities.end());
}

TEMPLATE
void POOL::kill(std::initializer_list<IdT> ids) noexcept {
	return kill(ids.begin(), ids.end());
}

TEMPLATE
bool POOL::contains(const IdT id) const noexcept {
	const size_t _id = id;
	const size_t pageNum = qdiv<Traits::pageSize>(_id);

	return pageNum < _sparse.size() and _sparse[pageNum] != nullptr and
		not Traits::Version::hasNull(_sparse[pageNum][qmod<Traits::pageSize>(_id)]);
}

TEMPLATE
bool POOL::alive(const EntityT entity) const noexcept {
	const size_t id = Traits::Id::part(entity);
	const size_t pageNum = qdiv<Traits::pageSize>(id);

	return pageNum < _sparse.size() and _sparse[pageNum] != nullptr and
		Traits::Version::rawPart(_sparse[pageNum][qmod<Traits::pageSize>(id)]) == Traits::Version::rawPart(entity);
}

TEMPLATE
POOL::Iterator POOL::find(const EntityT entity) noexcept {
	return find(Traits::Id::part(entity));
}

TEMPLATE
POOL::ConstIterator POOL::find(const EntityT entity) const noexcept {
	return find(Traits::Id::part(entity));
}

TEMPLATE
POOL::Iterator POOL::find(const IdT id) noexcept {
	if (id == Traits::Id::null) {
		return end();
	}

	const size_t _id = id;
	const size_t pageNum = qdiv<Traits::pageSize>(_id);

	return pageNum < _sparse.size() and _sparse[pageNum] != nullptr and
			not Traits::Version::hasNull(_sparse[pageNum][qmod<Traits::pageSize>(_id)]) ?
		begin() + Traits::Version::rawPart(_sparse[pageNum][qmod<Traits::pageSize>(_id)]) :
		end();
}

TEMPLATE
POOL::ConstIterator POOL::find(const IdT id) const noexcept {
	if (id == Traits::Id::null) {
		return end();
	}

	const size_t _id = id;
	const size_t pageNum = qdiv<Traits::pageSize>(_id);

	return pageNum < _sparse.size() and _sparse[pageNum] != nullptr and
			not Traits::Version::hasNull(_sparse[pageNum][qmod<Traits::pageSize>(_id)]) ?
		begin() + Traits::Version::rawPart(_sparse[pageNum][qmod<Traits::pageSize>(_id)]) :
		end();
}

TEMPLATE
POOL::VersionT POOL::version(const EntityT entity) const noexcept {
	return version(Traits::Id::part(entity));
}

TEMPLATE
POOL::VersionT POOL::version(const IdT id) const noexcept {
	return Traits::Version::part(contains(id) ? _sparseGet(id) : null);
}

TEMPLATE
size_t POOL::size() const noexcept {
	return _size;
}

} // namespace arch::ecs

#undef TEMPLATE
#undef POOL
