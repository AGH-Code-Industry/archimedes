#include "ComponentPool.h"
#include <utils/Assert.h>

#define TEMPLATE template<class C, class E>
#define POOL ComponentPool<C, E>

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
	if (_sparse.size() <= sparsePageNum) {
		while (_sparse.size() != sparsePageNum + 1) {
			_sparse.emplace_back();
		}
		//_sparse.resize(sparsePageNum + 1);
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
	if (_listHead == _dense.size()) {
		return { _dense.emplace_back(ETraits::Entity::null), _listHead++ };
	}

	if constexpr (Traits::inPlace) {
		const size_t idx = std::exchange(_listHead, ETraits::Id::part(_dense[_listHead]));
		return std::tie(_dense[idx], idx);
	} else {
		const size_t idx = _listHead++;
		return std::tie(_dense[idx], idx);
	}
}

TEMPLATE
C* POOL::_componentAssure(const IdT id) noexcept {
	const size_t pageNum = qdiv<Traits::pageSize>(id);
	if (_components.size() <= pageNum) {
		while (_components.size() != pageNum + 1) {
			_components.emplace_back();
		}
		// _components.resize(pageNum + 1);
	}

	auto&& pagePtr = _components[pageNum];
	if (pagePtr == nullptr) {
		pagePtr = reinterpret_cast<C*>(Traits::newPage());
	}

	return pagePtr + qmod<Traits::pageSize>(id);
}

TEMPLATE
E* POOL::_sparseGet(const IdT _id) noexcept {
	const size_t id = _id;
	const size_t pageNum = qdiv<ETraits::pageSize>(id);

	if (_sparse.size() < pageNum) {
		return nullptr;
	}

	auto page = _sparse[pageNum].get();

	return page ? page->data() + qmod<ETraits::pageSize>(id) : nullptr;
}

TEMPLATE
const E* POOL::_sparseGet(const IdT _id) const noexcept {
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
	}
	return _listHead - 1;
}

TEMPLATE
template<class... Args>
POOL::Reference POOL::addComponent(const EntityT entity, Args&&... args) noexcept {
	const auto id = ETraits::Id::part(entity);

	/*if (contains(id)) {
		return get(id);
	}*/

	auto&& sparseEntity = _sparseAssure(id);
	if (sparseEntity != ETraits::Entity::null) {
		const size_t idx = ETraits::Id::part(sparseEntity);
		return _components[qdiv<Traits::pageSize>(idx)][qmod<Traits::pageSize>(idx)];
	}

	auto&& [denseEntity, denseIdx] = _denseNew();

	const auto version = ETraits::Version::part(entity);

	sparseEntity = ETraits::Entity::fromParts(denseIdx, version);
	denseEntity = entity;

	auto&& componentPtr = _componentAssure(id);
	return *Traits::createAt(componentPtr, std::forward<Args>(args)...);
}

// TEMPLATE
// POOL::MoveReference POOL::removeComponent(
//	const EntityT entity,
//	MoveFlag
//) noexcept { // user must assume that component exists
//	const size_t id = ETraits::Id::part(entity);
//
//	const size_t sparsePageNum = qdiv<ETraits::pageSize>(id);
//	ARCH_ASSERT(_sparse.size() <= sparsePageNum, "removeComponent() with moveFlag assumes given entity has component");
//
//	auto&& sparsePagePtr = _sparse[sparsePageNum];
//	ARCH_ASSERT(sparsePagePtr, "removeComponent() with moveFlag assumes given entity has component");
//
//	const size_t sparsePageOffset = qmod<ETraits::pageSize>(id);
//	auto&& sparseEntity = sparsePagePtr[sparsePageOffset];
//	ARCH_ASSERT(
//		ETraits::Version::rawPart(sparseEntity) == ETraits::Version::rawPart(entity),
//		"removeComponent() with moveFlag assumes given entity has component"
//	);
//
//	const size_t idx = ETraits::Id::part(std::exchange(sparseEntity, ETraits::Entity::null));
//	_dense[idx] = ETraits::Entity::fromParts(std::exchange(_listHead, idx), ETraits::Version::null);
//
//	return std::move(_components[qdiv<Traits::pageSize>(idx)][qmod<Traits::pageSize>(idx)]);
// }

TEMPLATE
bool POOL::removeComponent(const EntityT entity) noexcept {
	const size_t id = ETraits::Id::part(entity);

	/*const size_t sparsePageNum = qdiv<ETraits::pageSize>(id);
	if (_sparse.size() <= sparsePageNum) {
		return false;
	}

	auto&& sparsePagePtr = _sparse[sparsePageNum];
	if (not sparsePagePtr) {
		return false
	}

	const size_t sparsePageOffset = qmod<ETraits::pageSize>(id);
	auto&& sparseEntity = sparsePagePtr[sparsePageOffset];
	if (ETraits::Version::rawPart(sparseEntity) != ETraits::Version::rawPart(entity)) {
		return false;
	}*/

	auto sparsePtr = _sparseGet(id);
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

		// first sparse swap
		// const size_t idx2 = std::exchange(
		sparseSwap = ETraits::Entity::fromRawParts(ETraits::Id::rawPart(entity), ETraits::Version::rawPart(sparseSwap));
		//);
		// second sparse swap
		const size_t idx = ETraits::Id::part(std::exchange(*sparsePtr, ETraits::Entity::null));

		_dense[idx] = _dense[_listHead];
		_dense[_listHead] = ETraits::Entity::fromParts(_listHead + 1, ETraits::Version::null);

		_components[qdiv<Traits::pageSize>(idx)][qmod<Traits::pageSize>(idx)] =
			std::move(_components[qdiv<Traits::pageSize>(_listHead)][qmod<Traits::pageSize>(_listHead)]);

		// swap probably unnecesary
		// move-assignment should take care of component's resources

		/*std::swap(
			_components[qdiv<Traits::pageSize>(idx)][qmod<Traits::pageSize>(idx)],
			_components[qdiv<Traits::pageSize>(_listHead)][qmod<Traits::pageSize>(_listHead)]
		);*/

		Traits::destroyAt(_components[qdiv<Traits::pageSize>(_listHead)] + qmod<Traits::pageSize>(_listHead));
	}

	/*const size_t idx = ETraits::Id::part(std::exchange(*sparsePtr, ETraits::Entity::null));
	_dense[idx] = ETraits::Entity::fromParts(std::exchange(_listHead, idx), ETraits::Version::null);
	Traits::destroyAt(_components[qdiv<Traits::pageSize>(idx)] + qmod<Traits::pageSize>(idx));*/

	return true;
}

TEMPLATE
bool POOL::contains(const EntityT entity) const noexcept {
	const size_t id = ETraits::Id::part(entity);

	const auto sparsePtr = _sparseGet(id);

	return sparsePtr and ETraits::Version::equal(*sparsePtr, entity);
}

TEMPLATE
bool POOL::contains(const IdT id) const noexcept {
	const auto sparsePtr = _sparseGet(id);

	return sparsePtr and not ETraits::Version::hasNull(*sparsePtr);
}

TEMPLATE
POOL::Reference POOL::get(const EntityT entity) noexcept {
	// assumed existence of component

	const size_t id = ETraits::Id::part(entity);

	auto sparsePtr = _sparseGet(id);
	ARCH_ASSERT(sparsePtr and ETraits::Version::equal(*sparsePtr, entity), "Component for entity not found");

	const size_t idx = ETraits::Id::part(*sparsePtr);

	return _components[qdiv<Traits::pageSize>(idx)][qmod<Traits::pageSize>(idx)];
}

TEMPLATE
POOL::ConstReference POOL::get(const EntityT entity) const noexcept {
	return const_cast<POOL*>(*this)->get(entity);
}

TEMPLATE
std::optional<std::reference_wrapper<C>> POOL::tryGet(const EntityT entity) noexcept {
	const size_t id = ETraits::Id::part(entity);

	auto sparsePtr = _sparseGet(id);
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
	/*if constexpr (Traits::inPlace) {
		auto iter = Iterator(this, _findFirst());
		return iter._valid() ? iter : ++iter;
	}*/
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
	return std::make_reverse_iterator(begin());
}

TEMPLATE
POOL::ConstReverseIterator POOL::rbegin() const noexcept {
	return std::make_reverse_iterator(begin());
}

TEMPLATE
POOL::ConstReverseIterator POOL::crbegin() const noexcept {
	return std::make_reverse_iterator(cbegin());
}

TEMPLATE
POOL::ReverseIterator POOL::rend() noexcept {
	return std::make_reverse_iterator(end());
}

TEMPLATE
POOL::ConstReverseIterator POOL::rend() const noexcept {
	return std::make_reverse_iterator(end());
}

TEMPLATE
POOL::ConstReverseIterator POOL::crend() const noexcept {
	return std::make_reverse_iterator(cend());
}

} // namespace arch::ecs

#undef TEMPLATE
#undef POOL
