#pragma once

#include <array>
#include <memory>
#include <optional>
#include <vector>

// #include "ComponentPoolData.h"
#include "ComponentPoolIterator.h"
#include "ComponentTraits.h"
#include <utils/MoveFlag.h>

namespace arch::ecs {

template<class C, class E>
class ComponentPool {
public:

	using Traits = _details::ComponentTraits<C, E>;
	using ETraits = _details::EntityTraits<E>;
	using EntityT = E;
	using IdT = typename ETraits::IdT;
	using VersionT = typename ETraits::VersionT;
	using ComponentT = C;
	using Reference = ComponentT&;
	using ConstReference = const ComponentT&;
	using MoveReference = ComponentT&&;

	using Iterator = _details::ComponentPoolIterator<C, E>;
	using ConstIterator = const Iterator;
	using ReverseIterator = std::reverse_iterator<Iterator>;
	using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

	~ComponentPool() noexcept;

	template<class... Args>
	Reference addComponent(const EntityT entity, Args&&... args) noexcept;
	// MoveReference removeComponent(const EntityT entity, MoveFlag) noexcept;
	bool removeComponent(const EntityT entity) noexcept;
	bool contains(const EntityT entity) const noexcept;
	bool contains(const IdT id) const noexcept;
	Reference get(const EntityT entity) noexcept;
	ConstReference get(const EntityT entity) const noexcept;
	std::optional<std::reference_wrapper<C>> tryGet(const EntityT entity) noexcept;
	std::optional<std::reference_wrapper<const C>> tryGet(const EntityT entity) const noexcept;
	// Reference get(const IdT entity) noexcept;
	// ConstReference get(const IdT id) const noexcept;

	Iterator begin() noexcept;
	ConstIterator begin() const noexcept;
	ConstIterator cbegin() const noexcept;
	Iterator end() noexcept;
	ConstIterator end() const noexcept;
	ConstIterator cend() const noexcept;
	ReverseIterator rbegin() noexcept;
	ConstReverseIterator rbegin() const noexcept;
	ConstReverseIterator crbegin() const noexcept;
	ReverseIterator rend() noexcept;
	ConstReverseIterator rend() const noexcept;
	ConstReverseIterator crend() const noexcept;

	auto entities() const noexcept;

private:

	friend arch::ecs::_details::ComponentPoolIterator<C, E>;

	E& _sparseAssure(const IdT id) noexcept;
	std::tuple<E&, size_t> _denseNew() noexcept;
	C* _componentAssure(const IdT id) noexcept;
	E* _sparseGet(const IdT _id) noexcept;
	const E* _sparseGet(const IdT _id) const noexcept;

	size_t _findFirst() const noexcept;
	size_t _findLast() const noexcept;

	using SparsePageT = std::array<E, ETraits::pageSize>;

	std::vector<std::unique_ptr<SparsePageT>> _sparse;
	std::vector<E> _dense;
	std::vector<C*> _components;
	size_t _listHead = 0;

public:

	std::tuple<decltype(_sparse)*, decltype(_dense)*, decltype(_components)*, decltype(_listHead)*> _debug() noexcept {
		return { &_sparse, &_dense, &_components, &_listHead };
	}
};

template<class E>
struct alignas(ComponentPool<void*, E>) ComponentPoolStorage {
	// void* is an arbitrary type
	using PoolT = ComponentPool<void*, E>;
	alignas(PoolT) std::array<char, sizeof(PoolT)> storage{};
};

} // namespace arch::ecs

#include "ComponentPool.hpp"
