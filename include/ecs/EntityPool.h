#pragma once

#include <concepts>
#include <memory>
#include <optional>
#include <vector>

#include "EntityTraits.h"

namespace arch::ecs {

/// @brief Pool for creating and killing entities
/// @details Uses sparse set data structure
/// @tparam E - entity type
template<class E>
class EntityPool {
	using SparseContainer = std::vector<std::unique_ptr<E[]>>; // paged container
	using DenseContainer = std::vector<E>;

public:

	/// @brief EntityTraits of entity
	using Traits = _details::EntityTraits<E>;
	/// @brief Entity type
	using EntityT = typename Traits::EntityT;
	/// @brief Entity id type
	using IdT = typename Traits::IdT;
	/// @brief Entity version type
	using VersionT = typename Traits::VersionT;

	static_assert(std::popcount(Traits::pageSize) == 1, "pageSize for entity must be a power of 2");

	/// @brief Iterator type
	using Iterator = typename DenseContainer::iterator;
	/// @brief Readonly iterator type
	using ConstIterator = typename DenseContainer::const_iterator;
	/// @brief Reverse iterator type
	using ReverseIterator = typename DenseContainer::reverse_iterator;
	/// @brief Readonly reverse iterator type
	using ConstReverseIterator = typename DenseContainer::const_reverse_iterator;

	/// @brief Null entity
	static inline constexpr EntityT null = Traits::Entity::null;

	void swap(EntityPool& other) noexcept;

	/// @brief Creates new entity or recycles killed
	/// @return Created entity, null if achieved entity limit
	EntityT newEntity() noexcept;
	/// @brief Recycles given entity, retaining its version
	/// @param entity - entity to recycle
	/// @return Recycled entity, null if id occupied
	EntityT recycleEntity(const EntityT entity) noexcept;
	/// @brief Recycles entity of given id
	/// @param id - id of entity to recycle
	/// @return Recycled entity, null if id occupied
	EntityT recycleId(IdT id) noexcept;

	/// @brief Kills given entity
	/// @param entity - entity to kill
	void kill(const EntityT entity) noexcept;
	/// @brief Kills entity of given id
	/// @param id - id of entity to kill
	void kill(const IdT id) noexcept;
	/// @brief Kills entities in range
	/// @param first - beginning of range
	/// @param last - end of range
	void kill(std::input_iterator auto first, std::input_iterator auto last) noexcept;
	/// @brief Kills all entites in list
	/// @param entities - entities to kill
	void kill(std::initializer_list<EntityT> entities) noexcept;
	/// @brief Kills all entites of ids in list
	/// @param entities - entities to kill
	void kill(std::initializer_list<IdT> ids) noexcept;

	/// @brief Checks if entity of given id is alive
	/// @param id - id of entity to check
	bool contains(const IdT id) const noexcept;
	/// @brief Checks if given entity is alive
	/// @param entity - entity to check
	bool alive(const EntityT entity) const noexcept;

	/// @brief Finds given entity
	/// @param entity - entity to find
	/// @return Iterator to found entity, end() otherwise
	Iterator find(const EntityT entity) noexcept;
	/// @brief Finds given entity
	/// @param entity - entity to find
	/// @return Iterator to found entity, end() otherwise
	ConstIterator find(const EntityT entity) const noexcept;
	/// @brief Finds entity of given id
	/// @param id - id of entity to find
	/// @return Iterator to found entity, end() otherwise
	Iterator find(IdT id) noexcept;
	/// @brief Finds entity of given id
	/// @param id - id of entity to find
	/// @return Iterator to found entity, end() otherwise
	ConstIterator find(IdT id) const noexcept;

	/// @brief Gets version of entity with the same id as given entity
	/// @param entity - entity to extract id from
	/// @return Version of entity, null version otherwise
	VersionT version(const EntityT entity) const noexcept;
	/// @brief Gets version of entity with given id
	/// @param id - id of entity to get version of
	/// @return Version of entity, null version otherwise
	VersionT version(const IdT id) const noexcept;

	/// @brief Returns amount of entities alive
	size_t size() const noexcept;

	/// @brief Returns iterator to the first entity
	Iterator begin() noexcept;
	/// @brief Returns readonly iterator to the first entity
	ConstIterator begin() const noexcept;
	/// @brief Returns readonly iterator to the first entity
	ConstIterator cbegin() const noexcept;
	/// @brief Returns iterator to the past-the-last entity
	Iterator end() noexcept;
	/// @brief Returns readonly iterator to the past-the-last entity
	ConstIterator end() const noexcept;
	/// @brief Returns readonly iterator to the past-the-last entity
	ConstIterator cend() const noexcept;
	/// @brief Returns reverse iterator to the first entity in reversed order
	ReverseIterator rbegin() noexcept;
	/// @brief Returns readonly reverse iterator to the first entity in reversed order
	ConstReverseIterator rbegin() const noexcept;
	/// @brief Returns readonly reverse iterator to the first entity in reversed order
	ConstReverseIterator crbegin() const noexcept;
	/// @brief Returns reverse iterator to the past-the-last entity in reversed order
	ReverseIterator rend() noexcept;
	/// @brief Returns readonly reverse iterator to the past-the-last entity in reversed order
	ConstReverseIterator rend() const noexcept;
	/// @brief Returns readonly reverse iterator to the past-the-last entity in reversed order
	ConstReverseIterator crend() const noexcept;

	std::tuple<SparseContainer*, DenseContainer*> debug() noexcept { return { &_sparse, &_dense }; }

private:

	// returns n-th sparse page, initializes if not exists
	EntityT* _tryInitPage(const size_t n) noexcept;

	// _tryInitPage(), but n is computed from id
	EntityT& _sparseAssure(const EntityT entity) noexcept;
	EntityT& _sparseAssure(const IdT id) noexcept;

	// obtains reference to entity in _sparse
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
