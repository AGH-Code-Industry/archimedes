#pragma once

#include <array>
#include <memory>
#include <optional>
#include <vector>

#include "ComponentPoolIterator.h"
#include "ComponentTraits.h"
#include <utils/MoveFlag.h>

namespace arch::ecs {

/// @brief Pool for creating and destroying components
/// @details Uses sparse set data structure
/// @tparam C - component type
/// @tparam E - entity type
template<class C, class E>
class ComponentPool {
public:

	/// @brief ComponentTraits of component
	using Traits = _details::ComponentTraits<C, E>;
	/// @brief EntityTraits of entity
	using ETraits = _details::EntityTraits<E>;
	/// @brief Entity type
	using EntityT = E;
	/// @brief Id type
	using IdT = typename ETraits::IdT;
	/// @brief Version type
	using VersionT = typename ETraits::VersionT;
	/// @brief Component type
	using ComponentT = C;
	/// @brief Reference to component
	using Reference = ComponentT&;
	/// @brief Readonly reference to component
	using ConstReference = const ComponentT&;
	/// @brief Move reference of component
	using MoveReference = ComponentT&&;

	/// @brief Iterator of component pool
	using Iterator = _details::ComponentPoolIterator<C, E>;
	/// @brief Const iterator of component pool
	using ConstIterator = const Iterator;
	/// @brief Reverse iterator of component pool
	using ReverseIterator = std::reverse_iterator<Iterator>;
	/// @brief Const reverse iterator of component pool
	using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

	/// @brief Destructor, deletes components.
	~ComponentPool() noexcept;

	/// @brief Adds component to given entity
	/// @param entity - entity to add component to
	/// @param ...args - argument for constructor
	/// @return Reference to added component
	template<class... Args>
	Reference addComponent(const EntityT entity, Args&&... args) noexcept;
	/// @brief Removes component from given entity, if has one
	/// @param entity - entity to remove component from
	/// @return Whether component was removed
	bool removeComponent(const EntityT entity) noexcept;
	/// @brief Removes and returns component from given entity, component must exist
	/// @details If component does not exist, the behavior is undefined
	/// @param entity - entity to remove component from
	/// @return Removed component
	C removeComponent(const EntityT entity, MoveFlag) noexcept requires std::movable<C>;
	/// @brief Checks if pool contains component for given entity
	/// @param entity - entity to check for
	bool contains(const EntityT entity) const noexcept;
	/// @brief Checks if pool contains component for given id
	/// @param id - id to check for
	bool contains(const IdT id) const noexcept;
	/// @brief Returns reference to component of given entity
	/// @details If component does not exist, the behavior is undefined
	/// @param entity - entity to get component of
	Reference get(const EntityT entity) noexcept;
	/// @brief Returns readonly reference to component of given entity
	/// @details If component does not exist, the behavior is undefined
	/// @param entity - entity to get component of
	ConstReference get(const EntityT entity) const noexcept;
	/// @brief Returns optional with reference to component of given entity
	/// @param entity - entity to get component of
	std::optional<std::reference_wrapper<C>> tryGet(const EntityT entity) noexcept;
	/// @brief Returns optional with readonly reference to component of given entity
	/// @param entity - entity to get component of
	std::optional<std::reference_wrapper<const C>> tryGet(const EntityT entity) const noexcept;

	/// @brief Returns iterator to first (const entity, component) contained
	Iterator begin() noexcept;
	/// @brief Returns const iterator to first (const entity, const component) contained
	ConstIterator begin() const noexcept;
	/// @brief Returns const iterator to first (const entity, const component) contained
	ConstIterator cbegin() const noexcept;
	/// @brief Returns iterator to past-the-last (const entity, component) contained
	Iterator end() noexcept;
	/// @brief Returns iterator to past-the-last (const entity, const component) contained
	ConstIterator end() const noexcept;
	/// @brief Returns iterator to past-the-last (const entity, const component) contained
	ConstIterator cend() const noexcept;
	/// @brief Returns reverse iterator to first (const entity, component) contained in a reverse order
	ReverseIterator rbegin() noexcept;
	/// @brief Returns reverse iterator to first (const entity, const component) contained in a reverse order
	ConstReverseIterator rbegin() const noexcept;
	/// @brief Returns reverse iterator to first (const entity, const component) contained in a reverse order
	ConstReverseIterator crbegin() const noexcept;
	/// @brief Returns reverse iterator to past-the-last (const entity, component) contained in a reverse order
	ReverseIterator rend() noexcept;
	/// @brief Returns reverse iterator to past-the-last (const entity, const component) contained in a reverse order
	ConstReverseIterator rend() const noexcept;
	/// @brief Returns reverse iterator to past-the-last (const entity, const component) contained in a reverse order
	ConstReverseIterator crend() const noexcept;

private:

	friend arch::ecs::_details::ComponentPoolIterator<C, E>;

	// returns entity of given id from sparse, assuring it's page exists
	E& _sparseAssure(const IdT id) noexcept;
	// returns new entity from dense, along with it's index
	std::tuple<E&, size_t> _denseNew() noexcept;
	// returns pointer (likely invalid) to component of given id, can be used for placement-new or Traits::constructAt()
	C* _componentAssure(const IdT id) noexcept;
	// returns pointer to sparse entity or nullptr if does not exist
	E* _sparseGetPtr(const IdT _id) noexcept;
	const E* _sparseGetPtr(const IdT _id) const noexcept;
	// returns reference to sparse entity, UB if does not exist
	E& _sparseGet(const IdT _id) noexcept;
	const E& _sparseGet(const IdT _id) const noexcept;

	// finds index of first valid component, 0 if none
	size_t _findFirst() const noexcept;
	// finds index of last valid component, 0 if none
	size_t _findLast() const noexcept;

	std::vector<std::unique_ptr<std::array<E, ETraits::pageSize>>> _sparse; // paged
	std::vector<E> _dense;
	std::vector<C*> _components; // paged
	size_t _listHead = 0;

	// for manual checks
	std::tuple<decltype(_sparse)*, decltype(_dense)*, decltype(_components)*, decltype(_listHead)*> _debug() noexcept {
		return { &_sparse, &_dense, &_components, &_listHead };
	}
};

/// @brief Struct mimicing ComponentPool of any component type
/// @brief Used to achieve simmilar behavior as std::any
/// @details Usage conforms with Strict Aliasing Rule, making it safe
/// @tparam E - entity type
template<class E>
struct alignas(ComponentPool<void*, E>) ComponentPoolStorage {
	// void* is an arbitrary type
	/// @brief Type of an arbitrary ComponentPool
	using PoolT = ComponentPool<void*, E>;
	/// @brief std::array with size and alignment of ComponentPool
	/// @brief char is an exception to Strict Aliasing Rule
	alignas(PoolT) std::array<char, sizeof(PoolT)> storage{};
};

} // namespace arch::ecs

#include "ComponentPool.hpp"
