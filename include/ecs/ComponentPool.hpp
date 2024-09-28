#include "ComponentPool.h"
#include <utils/Assert.h>

#define TEMPLATE_CE template<class C, class E>
#define POOL_CE ComponentPool<C, E>
#define TRAITS_CE _details::ComponentTraits<C, E>

// https://miro.com/app/board/uXjVK4gF1DI=/?share_link_id=296698570044
// ^ picture explanations

namespace arch::ecs {

TEMPLATE_CE
POOL_CE::~ComponentPool() noexcept {
	if constexpr (!Traits::flag) {
		for (size_t i = 0; i != _components.size(); ++i) {
			Traits::deletePage(_components.data(), i, _dense);
		}
	}
}

TEMPLATE_CE
std::tuple<E&, size_t> POOL_CE::_denseNew() noexcept {
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

TEMPLATE_CE
C* POOL_CE::_componentAssure(const IdT id) noexcept {
	const size_t pageNum = id / Traits::pageSize;

	// resize(n) makes capacity == n (bad)
	if (_components.size() < pageNum + 1) {
		_components.reserve(std::bit_ceil(pageNum + 1));
		_components.resize(pageNum + 1);
	}

	auto&& pagePtr = _components[pageNum];
	if (pagePtr == nullptr) {
		pagePtr = Traits::newPage();
	}

	return pagePtr + id % Traits::pageSize;
}

TEMPLATE_CE
size_t POOL_CE::_findFirst() const noexcept {
	if constexpr (Traits::inPlace) {
		for (size_t i = 0; i != _dense.size(); ++i) {
			if (!ETraits::Version::hasNull(_dense[i])) {
				return i;
			}
		}
		return 0;
	}
	return 0;
}

TEMPLATE_CE
size_t POOL_CE::_findLast() const noexcept {
	if constexpr (Traits::inPlace) {
		for (size_t i = _dense.size() - 1; i != (size_t)-1; --i) {
			if (!ETraits::Version::hasNull(_dense[i - 1])) {
				return i;
			}
		}
		return 0;
	}
	return _listHead - 1;
}

TEMPLATE_CE
template<class... Args>
POOL_CE::GetReference POOL_CE::addComponent(const EntityT entity, Args&&... args) noexcept {
	const auto id = ETraits::Id::part(entity);

	auto&& sparseEntity = _sparseAssure(id);
	if (sparseEntity != ETraits::Entity::null) {
		if constexpr (Traits::flag) {
			return false;
		} else {
			const size_t idx = ETraits::Id::part(sparseEntity);
			return _components[idx / Traits::pageSize][idx % Traits::pageSize];
		}
	}

	auto&& [denseEntity, denseIdx] = _denseNew();

	sparseEntity = ETraits::Entity::fromParts(denseIdx, ETraits::Version::part(entity));
	denseEntity = entity;
	++_counter;

	if constexpr (Traits::flag) {
		return true;
	} else {
		return Traits::constructAt(_componentAssure(id), std::forward<Args>(args)...);
	}
}

TEMPLATE_CE
C POOL_CE::removeComponent(const EntityT entity, MoveFlag) noexcept requires(std::movable<C> && !TRAITS_CE::flag)
{ // user must assume that component exists
	const size_t id = ETraits::Id::part(entity);

	ARCH_ASSERT(contains(entity), "Component for entity not found");

	auto&& fromSparse = _sparseGet(id);

	if constexpr (Traits::inPlace) {
		const size_t idx = ETraits::Id::part(std::exchange(fromSparse, ETraits::Entity::null));
		_dense[idx] = ETraits::Entity::fromParts(std::exchange(_listHead, idx), ETraits::Version::null);
		auto&& component = (*_components[idx / Traits::pageSize])[idx % Traits::pageSize];
		auto toMove = std::move(component);
		Traits::destroyAt(&component);
		--_counter;
		return toMove;
	} else {
		--_listHead;
		const size_t sparseSwapIdx = ETraits::Id::part(_dense[_listHead]);
		EntityT& sparseSwap = _sparse[sparseSwapIdx / ETraits::pageSize]->data()[sparseSwapIdx % ETraits::pageSize];

		// first sparse swap, id at listHead = id of given entity
		sparseSwap = ETraits::Entity::fromRawParts(ETraits::Id::rawPart(entity), ETraits::Version::rawPart(sparseSwap));
		// second sparse swap, entity at id = null
		// also obtain index to dense
		const size_t idx = ETraits::Id::part(std::exchange(fromSparse, ETraits::Entity::null));

		_dense[idx] = _dense[_listHead];
		_dense[_listHead] = ETraits::Entity::fromParts(_listHead + 1, ETraits::Version::null);

		auto&& atIdx = _components[idx / Traits::pageSize][idx % Traits::pageSize];
		auto&& atListHead = _components[_listHead / Traits::pageSize][_listHead % Traits::pageSize];

		auto toMove = std::move(atIdx);

		atIdx = std::move(atListHead);

		// swap probably unnecesary
		// move-assignment should take care of component's resources

		Traits::destroyAt(&atListHead);
		--_counter;

		// copy elision
		return toMove;
	}
}

TEMPLATE_CE
bool POOL_CE::removeComponent(const EntityT entity) noexcept {
	const size_t id = ETraits::Id::part(entity);

	auto sparsePtr = _sparseTryGet(id);
	if (!sparsePtr || *sparsePtr == ETraits::Entity::null) {
		return false;
	}

	if constexpr (Traits::inPlace) {
		const size_t idx = ETraits::Id::part(std::exchange(*sparsePtr, ETraits::Entity::null));
		_dense[idx] = ETraits::Entity::fromParts(std::exchange(_listHead, idx), ETraits::Version::null);
		if constexpr (!Traits::flag) {
			Traits::destroyAt(_components[idx / Traits::pageSize] + idx % Traits::pageSize);
		}
	} else {
		--_listHead;
		const size_t sparseSwapIdx = ETraits::Id::part(_dense[_listHead]);
		EntityT& sparseSwap = _sparse[sparseSwapIdx / ETraits::pageSize]->data()[sparseSwapIdx % ETraits::pageSize];

		// first sparse swap, id at listHead = id of given entity
		sparseSwap = ETraits::Entity::fromRawParts(ETraits::Id::rawPart(entity), ETraits::Version::rawPart(sparseSwap));

		// second sparse swap, entity at id = null
		// also obtain index to dense
		const size_t idx = ETraits::Id::part(std::exchange(*sparsePtr, ETraits::Entity::null));

		_dense[idx] = _dense[_listHead];
		_dense[_listHead] = ETraits::Entity::fromParts(_listHead + 1, ETraits::Version::null);
		--_counter;

		if constexpr (!Traits::flag) {
			auto&& atListHead = _components[_listHead / Traits::pageSize][_listHead % Traits::pageSize];

			_components[idx / Traits::pageSize][idx % Traits::pageSize] = std::move(atListHead);

			// swap probably unnecesary
			// move-assignment should take care of component's resources

			Traits::destroyAt(&atListHead);
		}
	}

	return true;
}

TEMPLATE_CE
POOL_CE::GetReference POOL_CE::get(const EntityT entity) noexcept {
	// assumed existence of component
	if constexpr (Traits::flag) {
		return contains(entity);
	} else {
		const size_t id = ETraits::Id::part(entity);

		ARCH_ASSERT(contains(entity), "Component for entity not found");

		const size_t idx = ETraits::Id::part(_sparseGet(id));

		return _components[idx / Traits::pageSize][idx % Traits::pageSize];
	}
}

TEMPLATE_CE
POOL_CE::ConstGetReference POOL_CE::get(const EntityT entity) const noexcept {
	return const_cast<POOL_CE*>(this)->get(entity);
}

TEMPLATE_CE
std::optional<std::reference_wrapper<C>> POOL_CE::tryGet(const EntityT entity) noexcept requires(!TRAITS_CE::flag)
{
	const size_t id = ETraits::Id::part(entity);

	auto sparsePtr = _sparseTryGet(id);
	if (!(sparsePtr && ETraits::Version::equal(*sparsePtr, entity))) {
		return std::nullopt;
	}

	const size_t idx = ETraits::Id::part(*sparsePtr);

	return std::optional{ std::ref(_components[idx / Traits::pageSize][idx % Traits::pageSize]) };
}

TEMPLATE_CE
std::optional<std::reference_wrapper<const C>> POOL_CE::tryGet(const EntityT entity) const noexcept
	requires(!TRAITS_CE::flag)
{
	return const_cast<POOL_CE*>(this)->tryGet(entity);
}

TEMPLATE_CE
POOL_CE::Iterator POOL_CE::begin() noexcept {
	return Iterator(this, _findFirst());
}

TEMPLATE_CE
const POOL_CE::Iterator POOL_CE::begin() const noexcept {
	return const_cast<POOL_CE*>(this)->begin();
}

TEMPLATE_CE
const POOL_CE::Iterator POOL_CE::cbegin() const noexcept {
	return begin();
}

TEMPLATE_CE
POOL_CE::Iterator POOL_CE::end() noexcept {
	return Iterator(this, _findLast() + 1);
}

TEMPLATE_CE
const POOL_CE::Iterator POOL_CE::end() const noexcept {
	return const_cast<POOL_CE*>(this)->end();
}

TEMPLATE_CE
const POOL_CE::Iterator POOL_CE::cend() const noexcept {
	return end();
}

TEMPLATE_CE
POOL_CE::ReverseIterator POOL_CE::rbegin() noexcept {
	return std::reverse_iterator(end());
}

TEMPLATE_CE
POOL_CE::ConstReverseIterator POOL_CE::rbegin() const noexcept {
	return std::reverse_iterator(end());
}

TEMPLATE_CE
POOL_CE::ConstReverseIterator POOL_CE::crbegin() const noexcept {
	return std::reverse_iterator(cend());
}

TEMPLATE_CE
POOL_CE::ReverseIterator POOL_CE::rend() noexcept {
	return std::reverse_iterator(begin());
}

TEMPLATE_CE
POOL_CE::ConstReverseIterator POOL_CE::rend() const noexcept {
	return std::reverse_iterator(begin());
}

TEMPLATE_CE
POOL_CE::ConstReverseIterator POOL_CE::crend() const noexcept {
	return std::reverse_iterator(cbegin());
}

} // namespace arch::ecs

#undef TEMPLATE_CE
#undef POOL_CE
#undef TRAITS_CE
