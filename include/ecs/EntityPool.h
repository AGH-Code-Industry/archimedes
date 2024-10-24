#pragma once

#include <concepts>
#include <memory>
#include <optional>
#include <vector>

#include "EntityTraits.h"
#include "SparseSet.h"

namespace arch::ecs {

/// @brief Pool for creating and killing entities
/// @details Uses sparse set data structure
/// @tparam Entity - entity type
class EntityPool: public _details::SparseSet {
	using Base = _details::SparseSet;

public:

	/// @brief EntityTraits of entity
	using Traits = _details::EntityTraits;
	/// @brief Entity type
	using EntityT = typename Traits::EntityT;
	/// @brief Entity id type
	using IdT = typename Traits::IdT;
	/// @brief Entity version type
	using VersionT = typename Traits::VersionT;

	/// @brief Iterator type
	using Iterator = typename Base::DenseContainer::iterator;
	/// @brief Const iterator type
	using ConstIterator = typename Base::DenseContainer::const_iterator;
	/// @brief Reverse iterator type
	using ReverseIterator = typename Base::DenseContainer::reverse_iterator;
	/// @brief Const reverse iterator type
	using ConstReverseIterator = typename Base::DenseContainer::const_reverse_iterator;

	/// @brief Null entity
	static inline constexpr EntityT null = Traits::Entity::null;

	/// @brief Swaps this pool with given pool
	/// @param other - pool to swap with
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

private:

	// for manual checks
	std::tuple<typename Base::SparseContainer*, typename Base::DenseContainer*> _debug() noexcept;

	size_t _size = 0;
};

} // namespace arch::ecs
