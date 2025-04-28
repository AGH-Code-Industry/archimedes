#pragma once

#include <array>
#include <memory>
#include <vector>

#include "EntityTraits.h"

namespace arch::ecs::_details {

/// @brief Sparse set data structure with find-like operations
/// @tparam Entity - entity type
class SparseSet {
public:

	/// @brief EntityTraits of entity
	using Traits = EntityTraits;
	/// @brief Entity type
	using EntityT = Traits::EntityT;
	/// @brief Id type
	using IdT = Traits::IdT;
	/// @brief Version type
	using VersionT = Traits::VersionT;

	/// @brief Find given entity
	/// @param entity - entity to find
	/// @return Index to dense if found
	/// @return (size_t)-1 if not found
	size_t find(const EntityT entity) const noexcept;
	/// @brief Find entity with given id
	/// @param id - id of entity to find
	/// @return Index to dense if found
	/// @return (size_t)-1 if not found
	size_t find(const IdT id) const noexcept;
	/// @brief Checks if given entity is contained
	/// @param entity - entity to check
	bool contains(const EntityT entity) const noexcept;
	/// @brief Checks if entity with given id is contained
	/// @param entity - entity to check
	bool contains(const IdT id) const noexcept;
	/// @brief Gets version of entity with equal id as given or null if not found
	/// @param entity - entity to check
	VersionT version(const EntityT entity) const noexcept;
	/// @brief Gets version of entity with given id or null if not found
	/// @param entity - entity to check
	VersionT version(const IdT id) const noexcept;

protected:

	using SparseContainer = std::vector<std::unique_ptr<Entity[]>>;
	using DenseContainer = std::vector<Entity>;

	// possibly inits sparse page
	EntityT* _sparseAssurePage(const size_t n) noexcept;
	// returns reference to sparse, inits if not found
	EntityT& _sparseAssure(const IdT id) noexcept;
	// returns reference to sparse, UB if not found
	EntityT& _sparseGet(const IdT id) noexcept;
	const EntityT& _sparseGet(const IdT id) const noexcept;
	// returns pointer to sparse, nullptr if not found
	EntityT* _sparseTryGet(const IdT id) noexcept;
	const EntityT* _sparseTryGet(const IdT id) const noexcept;

	SparseContainer _sparse;
	DenseContainer _dense;
};

} // namespace arch::ecs::_details
