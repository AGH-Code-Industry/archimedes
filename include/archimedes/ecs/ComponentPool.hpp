#include "ComponentPool.h"
#include "EntityOperators.h"
#include <archimedes/utils/Assert.h>

#define TEMPLATE_C template<class C>
#define POOL_C ComponentPool<C>
#define TRAITS_C _details::ComponentTraits<C>

// https://miro.com/app/board/uXjVK4gF1DI=/?share_link_id=296698570044
// ^ picture explanations

namespace arch::ecs {

TEMPLATE_C
POOL_C::~ComponentPool() noexcept {
	if constexpr (!Traits::flag) {
		for (size_t i = 0; i != _components.size(); ++i) {
			Traits::deletePage(_components.data(), i, _dense);
		}
	}
}

TEMPLATE_C
std::tuple<Entity&, size_t> POOL_C::_denseNew() noexcept {
	if (_listHead == _dense.size()) { // dense is full
		return { _dense.emplace_back(nullEntity), _listHead++ };
	}

	if constexpr (Traits::inPlace) {
		const size_t idx = std::exchange(_listHead, *_dense[_listHead]);
		return { _dense[idx], idx };
	} else {
		const size_t idx = _listHead++;
		return { _dense[idx], idx };
	}
}

TEMPLATE_C
C* POOL_C::_componentAssure(const IdT id) noexcept {
	const size_t pageNum = id / Traits::pageSize;

	// resize(n) only would make capacity == n (bad)
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

TEMPLATE_C
size_t POOL_C::_findFirst() const noexcept {
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

TEMPLATE_C
size_t POOL_C::_findLast() const noexcept {
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

TEMPLATE_C
template<class... Args>
POOL_C::GetReference POOL_C::addComponent(const EntityT entity, Args&&... args) noexcept {
	const auto id = *entity;

	auto&& sparseEntity = _sparseAssure(id);
	if (sparseEntity != ETraits::Ent::null) {
		if constexpr (Traits::flag) {
			return false;
		} else {
			const size_t idx = *sparseEntity;
			return _components[idx / Traits::pageSize][idx % Traits::pageSize];
		}
	}

	auto&& [denseEntity, denseIdx] = _denseNew();

	sparseEntity = ETraits::Ent::fromParts(denseIdx, ~entity);
	denseEntity = entity;
	++_counter;

	ARCH_ASSERT(contains(entity), "Component failed to add");

	if constexpr (Traits::flag) {
		return true;
	} else {
		return Traits::constructAt(_componentAssure(*sparseEntity), std::forward<Args>(args)...);
	}
}

TEMPLATE_C
C POOL_C::removeComponent(const EntityT entity, MoveFlag) noexcept requires(std::movable<C> && !TRAITS_C::flag)
{ // user must assume that component exists
	const size_t id = *entity;

	ARCH_ASSERT(contains(entity), "Component for entity not found");

	auto&& fromSparse = _sparseGet(id);

	if constexpr (Traits::inPlace) {
		const size_t idx = *std::exchange(fromSparse, ETraits::Ent::null);
		_dense[idx] = ETraits::Ent::fromParts(std::exchange(_listHead, idx), ETraits::Version::null);
		auto&& component = _components[idx / Traits::pageSize][idx % Traits::pageSize];
		auto toMove = std::move(component);
		Traits::destroyAt(&component);
		--_counter;
		return toMove;
	} else {
		--_listHead;
		const size_t sparseSwapIdx = *_dense[_listHead];
		EntityT& sparseSwap = _sparse[sparseSwapIdx / ETraits::pageSize][sparseSwapIdx % ETraits::pageSize];

		if (&sparseSwap != &fromSparse) {
			// first sparse swap, id at listHead = id of given entity
			sparseSwap =
				ETraits::Ent::fromRawParts(ETraits::Id::rawPart(fromSparse), ETraits::Version::rawPart(sparseSwap));
		}
		// second sparse swap, entity at id = null
		// also obtain index to dense
		const size_t idx = *std::exchange(fromSparse, ETraits::Ent::null);

		_dense[idx] = _dense[_listHead];
		_dense[_listHead] = ETraits::Ent::fromParts(_listHead + 1, ETraits::Version::null);

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

TEMPLATE_C
bool POOL_C::removeComponent(const EntityT entity) noexcept {
	const size_t id = *entity;

	auto sparsePtr = _sparseTryGet(id);
	if (!sparsePtr || *sparsePtr == ETraits::Ent::null) {
		return false;
	}

	if constexpr (Traits::inPlace) {
		const size_t idx = *std::exchange(*sparsePtr, ETraits::Ent::null);
		_dense[idx] = ETraits::Ent::fromParts(std::exchange(_listHead, idx), ETraits::Version::null);
		if constexpr (!Traits::flag) {
			Traits::destroyAt(_components[idx / Traits::pageSize] + idx % Traits::pageSize);
		}
	} else {
		--_listHead;
		const size_t sparseSwapIdx = *_dense[_listHead];
		EntityT& sparseSwap = _sparse[sparseSwapIdx / ETraits::pageSize][sparseSwapIdx % ETraits::pageSize];

		if (&sparseSwap != sparsePtr) {
			// first sparse swap, id at listHead = id of given entity
			sparseSwap =
				ETraits::Ent::fromRawParts(ETraits::Id::rawPart(*sparsePtr), ETraits::Version::rawPart(sparseSwap));
		}
		// second sparse swap, entity at id = null
		// also obtain index to dense
		const size_t idx = *std::exchange(*sparsePtr, ETraits::Ent::null);

		_dense[idx] = _dense[_listHead];
		_dense[_listHead] = ETraits::Ent::fromParts(_listHead + 1, ETraits::Version::null);

		if constexpr (!Traits::flag) {
			auto&& atListHead = _components[_listHead / Traits::pageSize][_listHead % Traits::pageSize];

			_components[idx / Traits::pageSize][idx % Traits::pageSize] = std::move(atListHead);

			// swap probably unnecesary
			// move-assignment should take care of component's resources

			Traits::destroyAt(&atListHead);
		}
	}
	--_counter;

	return true;
}

TEMPLATE_C
bool POOL_C::virtualRemoveComponent(const EntityT entity) noexcept {
	return removeComponent(entity);
}

TEMPLATE_C
POOL_C::GetReference POOL_C::get(const EntityT entity) noexcept {
	// assumed existence of component
	if constexpr (Traits::flag) {
		return contains(entity);
	} else {
		ARCH_ASSERT(contains(entity), "Component for entity not found");

		const size_t idx = *_sparseCGet(*entity);

		return _components[idx / Traits::pageSize][idx % Traits::pageSize];
	}
}

TEMPLATE_C
POOL_C::ConstGetReference POOL_C::get(const EntityT entity) const noexcept {
	return const_cast<POOL_C*>(this)->get(entity);
}

TEMPLATE_C
OptRef<C> POOL_C::tryGet(const EntityT entity) noexcept requires(!TRAITS_C::flag)
{
	const size_t id = *entity;

	auto sparsePtr = _sparseTryGet(id);
	if (!(sparsePtr && ~*sparsePtr == ~entity)) {
		return std::nullopt;
	}

	const size_t idx = **sparsePtr;

	return OptRef<C>{ _components[idx / Traits::pageSize][idx % Traits::pageSize] };
}

TEMPLATE_C
OptRef<const C> POOL_C::tryGet(const EntityT entity) const noexcept requires(!TRAITS_C::flag)
{
	return const_cast<POOL_C*>(this)->tryGet(entity);
}

TEMPLATE_C
POOL_C::Iterator POOL_C::begin() noexcept {
	return Iterator(this, _findFirst());
}

TEMPLATE_C
const POOL_C::Iterator POOL_C::begin() const noexcept {
	return const_cast<POOL_C*>(this)->begin();
}

TEMPLATE_C
const POOL_C::Iterator POOL_C::cbegin() const noexcept {
	return begin();
}

TEMPLATE_C
POOL_C::Iterator POOL_C::end() noexcept {
	return Iterator(this, _findLast() + 1);
}

TEMPLATE_C
const POOL_C::Iterator POOL_C::end() const noexcept {
	return const_cast<POOL_C*>(this)->end();
}

TEMPLATE_C
const POOL_C::Iterator POOL_C::cend() const noexcept {
	return end();
}

TEMPLATE_C
POOL_C::ReverseIterator POOL_C::rbegin() noexcept {
	return std::reverse_iterator(end());
}

TEMPLATE_C
POOL_C::ConstReverseIterator POOL_C::rbegin() const noexcept {
	return std::reverse_iterator(end());
}

TEMPLATE_C
POOL_C::ConstReverseIterator POOL_C::crbegin() const noexcept {
	return std::reverse_iterator(cend());
}

TEMPLATE_C
POOL_C::ReverseIterator POOL_C::rend() noexcept {
	return std::reverse_iterator(begin());
}

TEMPLATE_C
POOL_C::ConstReverseIterator POOL_C::rend() const noexcept {
	return std::reverse_iterator(begin());
}

TEMPLATE_C
POOL_C::ConstReverseIterator POOL_C::crend() const noexcept {
	return std::reverse_iterator(cbegin());
}

} // namespace arch::ecs

#undef TEMPLATE_C
#undef POOL_C
#undef TRAITS_C
