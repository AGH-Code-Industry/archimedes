#pragma once

#include "EntityPool.h"

#define TEMPLATE template<class E, size_t PageSize>
#define POOL EntityPool<E, PageSize>

namespace arch::ecs {

TEMPLATE
typename POOL::EntityT* POOL::_tryInitPage(const size_t n) noexcept {
	if (_sparse.size() <= n) {
		_sparse.resize(n + 1);
	}

	auto& page = _sparse[n];
	if (page == nullptr) {
		page = std::make_unique<EntityT[]>(PageSize);
		std::fill(page.get(), page.get() + PageSize, null);
	}

	return page.get();
}

TEMPLATE
typename POOL::EntityT& POOL::_sparseAssure(const EntityT entity) noexcept {
	const auto id = Traits::Id::part(entity);
	return _tryInitPage(qdiv<PageSize>(id))[qmod<PageSize>(id)];
}

TEMPLATE
typename POOL::EntityT& POOL::_sparseAssure(const IdT id) noexcept {
	return _tryInitPage(qdiv<PageSize>(id))[qmod<PageSize>(id)];
}

TEMPLATE
typename POOL::EntityT& POOL::_sparseGet(const EntityT entity) noexcept {
	const auto id = Traits::Id::part(entity);
	return _sparse[qdiv<PageSize>(id)][qmod<PageSize>(id)];
}

TEMPLATE
const typename POOL::EntityT& POOL::_sparseGet(const EntityT entity) const noexcept {
	const auto id = Traits::Id::part(entity);
	return _sparse[qdiv<PageSize>(id)][qmod<PageSize>(id)];
}

TEMPLATE
typename POOL::EntityT& POOL::_sparseGet(const IdT id) noexcept {
	return _sparse[qdiv<PageSize>(id)][qmod<PageSize>(id)];
}

TEMPLATE
const typename POOL::EntityT& POOL::_sparseGet(const IdT id) const noexcept {
	return _sparse[qdiv<PageSize>(id)][qmod<PageSize>(id)];
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
	if (not alive(entity)) {
		auto& sparse1 = _sparseGet(entity);
		auto& dense1 = _dense[Traits::Id::part(sparse1)];
		auto& dense2 = _dense[_size++];
		auto& sparse2 = _sparseGet(dense2);

		std::swap(dense1, dense2);
		Traits::Id::swap(sparse1, sparse2);

		sparse1 = Traits::Entity::fromOthers(sparse1, dense2);

		return dense2;
	}
	return null;
}

TEMPLATE
POOL::EntityT POOL::recycleId(const IdT id) noexcept {
	if (not contains(id)) {
		auto& sparse1 = _sparseGet(id);
		auto& dense1 = _dense[Traits::Id::part(sparse1)];
		auto& dense2 = _dense[_size++];
		auto& sparse2 = _sparseGet(dense2);

		std::swap(dense1, dense2);
		Traits::Id::swap(sparse1, sparse2);

		sparse1 = Traits::Entity::fromOthers(sparse1, dense2);

		return dense2;
	}
	return null;
}

TEMPLATE
void POOL::kill(EntityT entity) noexcept {
	if (alive(entity)) {
		auto& sparse1 = _sparseGet(entity);
		auto& dense1 = _dense[Traits::Id::part(sparse1)];
		auto& dense2 = _dense[_size - 1];
		auto& sparse2 = _sparseGet(dense2);

		std::swap(dense1, dense2);
		Traits::Id::swap(sparse1, sparse2);

		dense2 = Traits::Version::withNext(dense2);
		sparse1 = Traits::Version::withNull(sparse1);

		--_size;
	}
}

TEMPLATE
void POOL::kill(const IdT id) noexcept {
	if (contains(id)) {
		auto& sparse1 = _sparseGet(id);
		auto& dense1 = _dense[Traits::Id::part(sparse1)];
		auto& dense2 = _dense[_size - 1];
		auto& sparse2 = _sparseGet(dense2);

		std::swap(dense1, dense2);
		Traits::Id::swap(sparse1, sparse2);

		dense2 = Traits::Version::withNext(dense2);
		sparse1 = Traits::Version::withNull(sparse1);

		--_size;
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
	const size_t pageNum = qdiv<PageSize>(_id);

	return pageNum < _sparse.size() and _sparse[pageNum] != nullptr and
		not Traits::Version::hasNull(_sparse[pageNum][qmod<PageSize>(_id)]);
}

TEMPLATE
bool POOL::alive(const EntityT entity) const noexcept {
	const size_t id = Traits::Id::part(entity);
	const size_t pageNum = qdiv<PageSize>(id);

	return pageNum < _sparse.size() and _sparse[pageNum] != nullptr and
		Traits::Version::rawPart(_sparse[pageNum][qmod<PageSize>(id)]) == Traits::Version::rawPart(entity);
}

TEMPLATE
POOL::Iterator POOL::find(const EntityT entity) noexcept {
	const size_t id = Traits::Id::part(entity);
	const size_t pageNum = qdiv<PageSize>(id);

	return pageNum < _sparse.size() and _sparse[pageNum] != nullptr and
			Traits::Version::rawPart(_sparse[pageNum][qmod<PageSize>(id)]) == Traits::Version::rawPart(entity) ?
		begin() + Traits::Version::rawPart(_sparse[pageNum][qmod<PageSize>(id)]) :
		end();
}

TEMPLATE
POOL::ConstIterator POOL::find(const EntityT entity) const noexcept {
	const size_t id = Traits::Id::part(entity);
	const size_t pageNum = qdiv<PageSize>(id);

	return pageNum < _sparse.size() and _sparse[pageNum] != nullptr and
			Traits::Version::rawPart(_sparse[pageNum][qmod<PageSize>(id)]) == Traits::Version::rawPart(entity) ?
		begin() + Traits::Version::rawPart(_sparse[pageNum][qmod<PageSize>(id)]) :
		end();
}

TEMPLATE
POOL::Iterator POOL::find(const IdT id) noexcept {
	const size_t _id = id;
	const size_t pageNum = qdiv<PageSize>(_id);

	return pageNum < _sparse.size() and _sparse[pageNum] != nullptr and
			not Traits::Version::hasNull(_sparse[pageNum][qmod<PageSize>(_id)]) ?
		begin() + Traits::Version::rawPart(_sparse[pageNum][qmod<PageSize>(_id)]) :
		end();
}

TEMPLATE
POOL::ConstIterator POOL::find(const IdT id) const noexcept {
	const size_t _id = id;
	const size_t pageNum = qdiv<PageSize>(_id);

	return pageNum < _sparse.size() and _sparse[pageNum] != nullptr and
			not Traits::Version::hasNull(_sparse[pageNum][qmod<PageSize>(_id)]) ?
		begin() + Traits::Version::rawPart(_sparse[pageNum][qmod<PageSize>(_id)]) :
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
