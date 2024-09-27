#pragma once

#include <array>
#include <memory>
#include <optional>
#include <vector>

#include "CommonComponentPool.h"
#include "ComponentPoolIterator.h"
#include "ComponentTraits.h"
#include <utils/MoveFlag.h>

namespace arch::ecs {

/// @brief Pool for creating and destroying components
/// @details Uses sparse set data structure
/// @tparam C - component type
/// @tparam E - entity type
template<class C, class E>
class ComponentPool: public _details::CommonComponentPool<E> {
	using Base = _details::CommonComponentPool<E>;

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
	/// @brief Return type of get() method
	using GetReference = std::conditional_t<Traits::flag, bool, Reference>;
	/// @brief Return type of get() method
	using ConstGetReference = std::conditional_t<Traits::flag, bool, ConstReference>;

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
	/// @return Reference to added component or whether flag component was added
	template<class... Args>
	GetReference addComponent(const EntityT entity, Args&&... args) noexcept;
	/// @brief Removes component from given entity, if has one
	/// @param entity - entity to remove component from
	/// @return Whether component was removed
	bool removeComponent(const EntityT entity) noexcept override;
	/// @brief Removes and returns component from given entity, component must exist
	/// @details If component does not exist, the behavior is undefined
	/// @param entity - entity to remove component from
	/// @return Removed component
	C removeComponent(const EntityT entity, MoveFlag) noexcept
		requires(std::movable<C> and not _details::ComponentTraits<C, E>::flag);

	using Base::contains;
	using Base::count;

	/// @brief Returns component of given entity
	/// @details If component is non-flag and does not exist, the behavior is undefined
	/// @param entity - entity to get component of
	GetReference get(const EntityT entity) noexcept;
	/// @brief Returns readonly reference to component of given entity
	/// @details If component does not exist, the behavior is undefined
	/// @param entity - entity to get component of
	ConstGetReference get(const EntityT entity) const noexcept;
	/// @brief Returns optional with reference to component of given entity
	/// @param entity - entity to get component of
	std::optional<std::reference_wrapper<C>> tryGet(const EntityT entity) noexcept
		requires(!_details::ComponentTraits<C, E>::flag);
	/// @brief Returns optional with readonly reference to component of given entity
	/// @param entity - entity to get component of
	std::optional<std::reference_wrapper<const C>> tryGet(const EntityT entity) const noexcept
		requires(!_details::ComponentTraits<C, E>::flag);

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

	using Base::_entitiesForView;

private:

	using Base::_sparseAssure;
	using Base::_sparseAssurePage;
	using Base::_sparseGet;
	using Base::_sparseTryGet;

	friend arch::ecs::_details::ComponentPoolIterator<C, E>;

	//// returns entity of given id from sparse, assuring it's page exists
	// E& _sparseAssure(const IdT id) noexcept;
	//  returns new entity from dense, along with it's index
	std::tuple<E&, size_t> _denseNew() noexcept;
	// returns pointer (likely invalid) to component of given id, can be used for placement-new or Traits::constructAt()
	C* _componentAssure(const IdT id) noexcept;

	// finds index of first valid component, 0 if none
	size_t _findFirst() const noexcept;
	// finds index of last valid component, 0 if none
	size_t _findLast() const noexcept;

	// std::vector<std::unique_ptr<std::array<E, ETraits::pageSize>>> _sparse; // paged
	// std::vector<E> _dense;
	using Base::_counter;
	using Base::_dense;
	using Base::_sparse;
	std::vector<C*> _components; // paged
	size_t _listHead = 0;

	// for manual checks
	std::tuple<
		typename Base::SparseContainer*,
		typename Base::DenseContainer*,
		decltype(_components)*,
		decltype(_listHead)*>
	_debug() noexcept {
		return { &this->_sparse, &this->_dense, &_components, &_listHead };
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
