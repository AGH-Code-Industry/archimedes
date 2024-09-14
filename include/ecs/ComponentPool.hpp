#include "ComponentPool.h"
#include <utils/Assert.h>

#define TEMPLATE template<class C, class E>
#define POOL ComponentPool<C, E>

// https://miro.com/app/board/uXjVK4gF1DI=/?share_link_id=296698570044
// ^ picture explanations

namespace arch::ecs {

TEMPLATE
POOL::~ComponentPool() noexcept {
	for (size_t i = 0; i != _components.size(); ++i) {
		Traits::deletePage(_components.data(), i, _dense);
	}
}

TEMPLATE
E& POOL::_sparseAssure(const IdT id) noexcept {
	const size_t sparsePageNum = qdiv<ETraits::pageSize>(id);
	// virtually equal to _sparse.resize(sparsePageNum + 1), but capacity >= sparsePageNum + 1
	while (_sparse.size() != sparsePageNum + 1) {
		_sparse.emplace_back();
	}

	auto&& sparsePagePtr = _sparse[sparsePageNum];
	if (sparsePagePtr == nullptr) {
		sparsePagePtr = std::make_unique<std::array<E, ETraits::pageSize>>();
		sparsePagePtr->fill(ETraits::Entity::null);
	}

	return (*sparsePagePtr)[qmod<ETraits::pageSize>(id)];
}

TEMPLATE
std::tuple<E&, size_t> POOL::_denseNew() noexcept {
	if (_listHead == _dense.size()) { // dense is full
		return { _dense.emplace_back(ETraits::Entity::null), _listHead++ };
	}

	if constexpr (Traits::inPlace) {
		const size_t idx = std::exchange(_listHead, ETraits::Id::part(_dense[_listHead]));
		return { _dense[idx], idx };
	} else {
		const size_t idx = _listHead++;
		return { _dense[idx], idx };
	}
}

TEMPLATE
C* POOL::_componentAssure(const IdT id) noexcept {
	const size_t pageNum = qdiv<Traits::pageSize>(id);
	// virtually equal to __components.resize(pageNum + 1), but capacity >= pageNum + 1
	while (_components.size() != pageNum + 1) {
		_components.emplace_back();
	}

	auto&& pagePtr = _components[pageNum];
	if (pagePtr == nullptr) {
		pagePtr = Traits::newPage();
	}

	return pagePtr + qmod<Traits::pageSize>(id);
}

TEMPLATE
E* POOL::_sparseGetPtr(const IdT _id) noexcept {
	const size_t id = _id;
	const size_t pageNum = qdiv<ETraits::pageSize>(id);

	if (_sparse.size() <= pageNum) {
		return nullptr;
	}

	auto page = _sparse[pageNum].get();

	return page ? page->data() + qmod<ETraits::pageSize>(id) : nullptr;
}

TEMPLATE
const E* POOL::_sparseGetPtr(const IdT _id) const noexcept {
	return const_cast<POOL*>(this)->_sparseGetPtr(_id);
}

TEMPLATE
E& POOL::_sparseGet(const IdT _id) noexcept {
	const size_t id = _id;

	ARCH_ASSERT(
		qdiv<ETraits::pageSize>(id) < _sparse.size() and _sparse[qdiv<ETraits::pageSize>(id)].get(),
		"Sparse page for given id does not exist"
	);

	return (*_sparse[qdiv<ETraits::pageSize>(id)])[qmod<ETraits::pageSize>(id)];
}

TEMPLATE
const E& POOL::_sparseGet(const IdT _id) const noexcept {
	return const_cast<POOL*>(this)->_sparseGet(_id);
}

TEMPLATE
size_t POOL::_findFirst() const noexcept {
	if constexpr (Traits::inPlace) {
		for (size_t i = 0; i != _dense.size(); ++i) {
			if (not ETraits::Version::hasNull(_dense[i])) {
				return i;
			}
		}
		return 0;
	}
	return 0;
}

TEMPLATE
size_t POOL::_findLast() const noexcept {
	if constexpr (Traits::inPlace) {
		for (size_t i = _dense.size() - 1; i != (size_t)-1; --i) {
			if (not ETraits::Version::hasNull(_dense[i - 1])) {
				return i;
			}
		}
		return 0;
	}
	return _listHead - 1;
}

TEMPLATE
template<class... Args>
POOL::Reference POOL::addComponent(const EntityT entity, Args&&... args) noexcept {
	const auto id = ETraits::Id::part(entity);

	auto&& sparseEntity = _sparseAssure(id);
	if (sparseEntity != ETraits::Entity::null) {
		const size_t idx = ETraits::Id::part(sparseEntity);
		return _components[qdiv<Traits::pageSize>(idx)][qmod<Traits::pageSize>(idx)];
	}

	auto&& [denseEntity, denseIdx] = _denseNew();

	sparseEntity = ETraits::Entity::fromParts(denseIdx, ETraits::Version::part(entity));
	denseEntity = entity;

	return Traits::constructAt(_componentAssure(id), std::forward<Args>(args)...);
}

TEMPLATE
C POOL::removeComponent(
	const EntityT entity,
	MoveFlag
) noexcept requires std::movable<C>
{ // user must assume that component exists
	const size_t id = ETraits::Id::part(entity);

	ARCH_ASSERT(contains(entity), "Component for entity not found");

	auto&& fromSparse = _sparseGet(id);

	if constexpr (Traits::inPlace) {
		const size_t idx = ETraits::Id::part(std::exchange(fromSparse, ETraits::Entity::null));
		_dense[idx] = ETraits::Entity::fromParts(std::exchange(_listHead, idx), ETraits::Version::null);
		auto&& component = (*_components[qdiv<Traits::pageSize>(idx)])[qmod<Traits::pageSize>(idx)];
		auto toMove = std::move(component);
		Traits::destroyAt(&component);
		return toMove;
	} else {
		--_listHead;
		const size_t sparseSwapIdx = ETraits::Id::part(_dense[_listHead]);
		EntityT& sparseSwap =
			_sparse[qdiv<ETraits::pageSize>(sparseSwapIdx)]->data()[qmod<ETraits::pageSize>(sparseSwapIdx)];

		// first sparse swap, id at listHead = id of given entity
		sparseSwap = ETraits::Entity::fromRawParts(ETraits::Id::rawPart(entity), ETraits::Version::rawPart(sparseSwap));
		// second sparse swap, entity at id = null
		// also obtain index to dense
		const size_t idx = ETraits::Id::part(std::exchange(fromSparse, ETraits::Entity::null));

		_dense[idx] = _dense[_listHead];
		_dense[_listHead] = ETraits::Entity::fromParts(_listHead + 1, ETraits::Version::null);

		auto&& atIdx = _components[qdiv<Traits::pageSize>(idx)][qmod<Traits::pageSize>(idx)];
		auto&& atListHead = _components[qdiv<Traits::pageSize>(_listHead)][qmod<Traits::pageSize>(_listHead)];

		auto toMove = std::move(atIdx);

		atIdx = std::move(atListHead);

		// swap probably unnecesary
		// move-assignment should take care of component's resources

		Traits::destroyAt(&atListHead);

		// copy elision
		return toMove;
	}
}

TEMPLATE
bool POOL::removeComponent(const EntityT entity) noexcept {
	const size_t id = ETraits::Id::part(entity);

	auto sparsePtr = _sparseGetPtr(id);
	if (not sparsePtr or *sparsePtr == ETraits::Entity::null) {
		return false;
	}

	if constexpr (Traits::inPlace) {
		const size_t idx = ETraits::Id::part(std::exchange(*sparsePtr, ETraits::Entity::null));
		_dense[idx] = ETraits::Entity::fromParts(std::exchange(_listHead, idx), ETraits::Version::null);
		Traits::destroyAt(_components[qdiv<Traits::pageSize>(idx)] + qmod<Traits::pageSize>(idx));
	} else {
		--_listHead;
		const size_t sparseSwapIdx = ETraits::Id::part(_dense[_listHead]);
		EntityT& sparseSwap =
			_sparse[qdiv<ETraits::pageSize>(sparseSwapIdx)]->data()[qmod<ETraits::pageSize>(sparseSwapIdx)];

		// first sparse swap, id at listHead = id of given entity
		sparseSwap = ETraits::Entity::fromRawParts(ETraits::Id::rawPart(entity), ETraits::Version::rawPart(sparseSwap));

		// second sparse swap, entity at id = null
		// also obtain index to dense
		const size_t idx = ETraits::Id::part(std::exchange(*sparsePtr, ETraits::Entity::null));

		_dense[idx] = _dense[_listHead];
		_dense[_listHead] = ETraits::Entity::fromParts(_listHead + 1, ETraits::Version::null);

		auto&& atListHead = _components[qdiv<Traits::pageSize>(_listHead)][qmod<Traits::pageSize>(_listHead)];

		_components[qdiv<Traits::pageSize>(idx)][qmod<Traits::pageSize>(idx)] = std::move(atListHead);

		// swap probably unnecesary
		// move-assignment should take care of component's resources

		Traits::destroyAt(&atListHead);
	}

	return true;
}

TEMPLATE
bool POOL::contains(const EntityT entity) const noexcept {
	const auto sparsePtr = _sparseGetPtr(ETraits::Id::part(entity));

	return sparsePtr and ETraits::Version::equal(*sparsePtr, entity);
}

TEMPLATE
bool POOL::contains(const IdT id) const noexcept {
	const auto sparsePtr = _sparseGetPtr(id);

	return sparsePtr and not ETraits::Version::hasNull(*sparsePtr);
}

TEMPLATE
POOL::Reference POOL::get(const EntityT entity) noexcept {
	// assumed existence of component

	const size_t id = ETraits::Id::part(entity);

	ARCH_ASSERT(contains(entity), "Component for entity not found");

	const size_t idx = ETraits::Id::part(_sparseGet(id));

	return _components[qdiv<Traits::pageSize>(idx)][qmod<Traits::pageSize>(idx)];
}

TEMPLATE
POOL::ConstReference POOL::get(const EntityT entity) const noexcept {
	return const_cast<POOL*>(*this)->get(entity);
}

TEMPLATE
std::optional<std::reference_wrapper<C>> POOL::tryGet(const EntityT entity) noexcept {
	const size_t id = ETraits::Id::part(entity);

	auto sparsePtr = _sparseGetPtr(id);
	if (not(sparsePtr and ETraits::Version::equal(*sparsePtr, entity))) {
		return std::nullopt;
	}

	const size_t idx = ETraits::Id::part(*sparsePtr);

	return std::optional{ std::ref(_components[qdiv<Traits::pageSize>(idx)][qmod<Traits::pageSize>(idx)]) };
}

TEMPLATE
std::optional<std::reference_wrapper<const C>> POOL::tryGet(const EntityT entity) const noexcept {
	return const_cast<POOL*>(this)->tryGet(entity);
}

TEMPLATE
POOL::Iterator POOL::begin() noexcept {
	return Iterator(this, _findFirst());
}

TEMPLATE
const POOL::Iterator POOL::begin() const noexcept {
	return const_cast<POOL*>(this)->begin();
}

TEMPLATE
const POOL::Iterator POOL::cbegin() const noexcept {
	return begin();
}

TEMPLATE
POOL::Iterator POOL::end() noexcept {
	return Iterator(this, _findLast() + 1);
}

TEMPLATE
const POOL::Iterator POOL::end() const noexcept {
	return const_cast<POOL*>(this)->end();
}

TEMPLATE
const POOL::Iterator POOL::cend() const noexcept {
	return end();
}

TEMPLATE
POOL::ReverseIterator POOL::rbegin() noexcept {
	return std::reverse_iterator(end());
}

TEMPLATE
POOL::ConstReverseIterator POOL::rbegin() const noexcept {
	return std::reverse_iterator(end());
}

TEMPLATE
POOL::ConstReverseIterator POOL::crbegin() const noexcept {
	return std::reverse_iterator(cend());
}

TEMPLATE
POOL::ReverseIterator POOL::rend() noexcept {
	return std::reverse_iterator(begin());
}

TEMPLATE
POOL::ConstReverseIterator POOL::rend() const noexcept {
	return std::reverse_iterator(begin());
}

TEMPLATE
POOL::ConstReverseIterator POOL::crend() const noexcept {
	return std::reverse_iterator(cbegin());
}

} // namespace arch::ecs

#undef TEMPLATE
#undef POOL
