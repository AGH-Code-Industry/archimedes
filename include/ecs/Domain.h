#pragma once

#include <concepts>
#include <unordered_map>

#include "ComponentPool.h"
#include "EntityPool.h"
#include "ExcludeT.h"
#include "View.h"
#include "meta/Rtti.h"
#include "tUtils/TypeString.h"

namespace arch::ecs {

/// @brief ECS' domain (main class)
/// @tparam E - entity type
template<class E>
class Domain {
public:

	/// @brief EntityTraits of entity
	using Traits = _details::EntityTraits<E>;
	/// @brief Entity type
	using EntityT = typename Traits::EntityT;
	/// @brief Id type
	using IdT = typename Traits::IdT;
	/// @brief Version type
	using VersionT = typename Traits::VersionT;

	/// @brief Iterator type
	using Iterator = typename EntityPool<E>::Iterator;
	/// @brief Const iterator type
	using ConstIterator = typename EntityPool<E>::ConstIterator;
	/// @brief Reverse iterator type
	using ReverseIterator = typename EntityPool<E>::ReverseIterator;
	/// @brief Const reverse iterator type
	using ConstReverseIterator = typename EntityPool<E>::ConstReverseIterator;
	template<class C>
	using GetReference = std::conditional_t<_details::ComponentTraits<std::remove_const_t<C>, E>::flag, bool, C&>;
	template<class C>
	using ConstGetReference = std::
		conditional_t<_details::ComponentTraits<std::remove_const_t<C>, E>::flag, bool, const std::remove_const_t<C>&>;

	/// @brief Default constructor
	Domain() noexcept = default;
	/// @brief Destructor
	~Domain() noexcept;
	/// @brief Copy constructor
	Domain(const Domain&) noexcept = default;
	/// @brief Move constructor
	Domain(Domain&&) noexcept = default;
	/// @brief Copy-assignment operator
	Domain& operator=(const Domain&) noexcept = default;
	/// @brief Move-assignment operator
	Domain& operator=(Domain&&) noexcept = default;

	/// @brief Swaps with the other Domain
	/// @param other - domain to swap
	void swap(Domain& other) noexcept;

	/// @brief Checks if given entity is alive
	/// @param entity - entity to check
	bool alive(const EntityT entity) const noexcept;
	/// @brief Checks if given entity of given id is alive
	/// @param id - id of entity to check
	bool contains(const IdT id) const noexcept;
	/// @brief Returns version of entity with id of given entity
	/// @param entity - entity to check
	VersionT version(const EntityT entity) const noexcept;
	/// @brief Returns version of entity with given id
	/// @param id - id of entity to check
	VersionT version(const IdT id) const noexcept;
	/// @brief Returns count of entities alive
	size_t count() const noexcept;

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
	EntityT recycleId(const IdT id) noexcept;

	/// @brief Kills given entity
	/// @param entity - entity to kill
	void kill(const EntityT entity) noexcept;
	/// @brief Kills entities in range
	/// @param first - beginning of range
	/// @param last - end of range
	void kill(std::input_iterator auto first, std::input_iterator auto last) noexcept;
	/// @brief Kills all entites in list
	/// @param entities - entities to kill
	void kill(std::initializer_list<EntityT> entities) noexcept;

	/// @brief Returns readonly std::view of entities
	auto entities() const noexcept;

	/// @brief Adds component to entity or returns existing one
	/// @tparam C - component type
	/// @param entity - entity to add component to
	/// @param ...args - arguments to constructor
	/// @return Reference to new entity or old one
	template<class C, class... Args>
	GetReference<std::remove_const_t<C>> addComponent(const EntityT entity, Args&&... args) noexcept;
	/// @brief Obtains reference to existing component of given entity
	/// @details If entity does not contain component, behavior is undefined
	/// @tparam C - component type
	/// @param entity - entity to get component from
	template<class C>
	requires(!std::is_const_v<C>)
	GetReference<C> getComponent(const EntityT entity) noexcept;
	/// @brief Obtains readonly reference to existing component of given entity
	/// @details If entity does not contain component, behavior is undefined
	/// @tparam C - component type
	/// @param entity - entity to get component from
	template<class C>
	ConstGetReference<std::remove_const_t<C>> getComponent(const EntityT entity) const noexcept;
	/// @brief Obtains readonly reference to existing component of given entity
	/// @details If entity does not contain component, behavior is undefined
	/// @tparam C - component type
	/// @param entity - entity to get component from
	template<class C>
	requires(std::is_const_v<C>)
	ConstGetReference<std::remove_const_t<C>> getComponent(const EntityT entity) const noexcept;
	/// @brief Obtains optional with reference to component of given entity
	/// @tparam C - component type
	/// @param entity - entity to get component from
	template<class C>
	requires(!_details::ComponentTraits<C, E>::flag && !std::is_const_v<C>)
	std::optional<std::reference_wrapper<C>> tryGetComponent(const EntityT entity) noexcept;
	/// @brief Obtains optional with readonly reference to component of given entity
	/// @tparam C - component type
	/// @param entity - entity to get component from
	template<class C>
	requires(!_details::ComponentTraits<std::remove_const_t<C>, E>::flag)
	std::optional<std::reference_wrapper<const std::remove_const_t<C>>> tryGetComponent(const EntityT entity
	) const noexcept;
	/// @brief Removes component from given entity, if has one
	/// @param entity - entity to remove component from
	/// @return Whether component was removed
	template<class C>
	bool removeComponent(const EntityT entity) noexcept;
	/// @brief Removes and returns component from given entity, component must exist
	/// @details If component does not exist, the behavior is undefined
	/// @param entity - entity to remove component from
	/// @return Removed component
	template<class C>
	requires(std::movable<std::remove_const_t<C>> && !_details::ComponentTraits<std::remove_const_t<C>, E>::flag)
	std::remove_const_t<C> removeComponent(const EntityT entity, MoveFlag) noexcept;

	/// @brief Checks if entity has component
	/// @tparam C - component type
	/// @param entity - entity to check
	template<class C>
	bool hasComponent(const EntityT entity) const noexcept;
	/// @brief Returns count of given component type
	/// @tparam C - component to count
	template<class C>
	size_t count() const noexcept;
	/// @brief Returns std::view of components of given type
	/// @tparam C - component type
	template<class C>
	requires(!std::is_const_v<C>)
	auto components() noexcept;
	/// @brief Returns std::view of readonly components of given type
	/// @tparam C - component type
	template<class C>
	auto components() const noexcept;

	/// @brief Returns view of given components
	/// @tparam Includes - components included in view
	/// @param <unnamed> - instance of ExcludeT<Excludes...>, where Excludes are components to be excluded from view
	template<class... Includes, class... Excludes>
	auto view(ExcludeT<Excludes...> = ExcludeT{}) noexcept;
	/// @brief Returns readonly view of given components
	/// @tparam Includes - components included in view
	/// @param <unnamed> (optional) - instance of ExcludeT<Excludes...>, where Excludes are components to be excluded
	/// from view
	template<class... Includes, class... Excludes>
	auto view(ExcludeT<Excludes...> = ExcludeT{}) const noexcept;
	/// @brief Explicitly returns readonly view of given components
	/// @tparam Includes - components included in view
	/// @param <unnamed> (optional) - instance of ExcludeT<Excludes...>, where Excludes are components to be excluded
	/// from view
	template<class... Includes, class... Excludes>
	auto readonlyView(ExcludeT<Excludes...> = ExcludeT{}) const noexcept;

private:

	// ComponentPools mapped by type
	using CPoolsT = std::unordered_map<TypeDescriptorWrapper, ComponentPoolStorage<E>>;

	// returns ComponentPool for given type, initializes if not exists
	template<class C>
	ComponentPool<std::remove_const_t<C>, E>& _assureCPool() noexcept;
	// returns ComponentPool for given type
	template<class C>
	requires(!std::is_const_v<C>)
	ComponentPool<C, E>& _getCPool() noexcept;
	template<class C>
	const ComponentPool<std::remove_const_t<C>, E>& _getCPool() const noexcept;
	// same as ComponentPool, but nullptr if does not exist
	template<class C>
	requires(!std::is_const_v<C>)
	ComponentPool<C, E>* _tryGetCPool() noexcept;
	template<class C>
	const ComponentPool<std::remove_const_t<C>, E>* _tryGetCPool() const noexcept;
	// destroying function for ComponentPool of given type
	template<class C>
	static inline void _destroyCPool(CPoolsT& cpools) noexcept;

	EntityPool<E> _entityPool;
	CPoolsT _componentPools;
	// mapped destroying functions
	std::unordered_map<TypeDescriptorWrapper, void (*)(CPoolsT&)> _cpoolDestroyers;
};

} // namespace arch::ecs

#include "Domain.hpp"

