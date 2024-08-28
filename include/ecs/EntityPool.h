#pragma once

#include <concepts>
#include <memory>
#include <optional>
#include <vector>

#include "EntityTraits.h"

namespace arch::ecs {

template<class E, size_t PageSize = 1'024>
class EntityPool {
	using SparseContainer = std::vector<std::unique_ptr<E[]>>;
	using DenseContainer = std::vector<E>;

public:

	using Traits = _details::EntityTraits<E>;
	using EntityT = typename Traits::EntityT;
	using IdT = typename Traits::IdT;
	using VersionT = typename Traits::VersionT;

	using Iterator = typename DenseContainer::iterator;
	using ConstIterator = typename DenseContainer::const_iterator;
	using ReverseIterator = typename DenseContainer::reverse_iterator;
	using ConstReverseIterator = typename DenseContainer::const_reverse_iterator;

	static inline constexpr EntityT null = Traits::Entity::null;

	EntityT newEntity() noexcept;
	EntityT recycleEntity(const EntityT entity) noexcept;
	EntityT recycleId(IdT id) noexcept;
	void kill(const EntityT entity) noexcept;
	void kill(const IdT id) noexcept;
	void kill(std::input_iterator auto first, std::input_iterator auto last) noexcept;
	void kill(std::initializer_list<EntityT> entities) noexcept;
	void kill(std::initializer_list<IdT> ids) noexcept;

	bool contains(const IdT id) const noexcept;
	bool alive(const EntityT entity) const noexcept;
	Iterator find(const EntityT entity) noexcept;
	ConstIterator find(const EntityT entity) const noexcept;
	Iterator find(IdT id) noexcept;
	ConstIterator find(IdT id) const noexcept;
	VersionT version(const EntityT entity) const noexcept;
	VersionT version(const IdT id) const noexcept;
	size_t size() const noexcept;

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

	std::tuple<SparseContainer*, DenseContainer*> debug() noexcept { return { &_sparse, &_dense }; }

private:

	EntityT* _tryInitPage(const size_t n) noexcept;
	EntityT& _sparseAssure(const EntityT entity) noexcept;
	EntityT& _sparseAssure(const IdT id) noexcept;
	EntityT& _sparseGet(const EntityT entity) noexcept;
	const EntityT& _sparseGet(const EntityT entity) const noexcept;
	EntityT& _sparseGet(const IdT id) noexcept;
	const EntityT& _sparseGet(const IdT id) const noexcept;

	SparseContainer _sparse;
	DenseContainer _dense;
	size_t _size = 0;
};

} // namespace arch::ecs

#include "EntityPool.hpp"
