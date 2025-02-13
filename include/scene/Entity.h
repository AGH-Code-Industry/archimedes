#pragma once

#include "ChildrenIterator.h"
#include "HierarchyIterator.h"
#include "NameComponent.h"
#include <Hier.h>
#include <utils/BoundedRange.h>
#include <utils/Res.h>
#include <utils/UUID.h>

#define ARCH_CTRAITS arch::ecs::_details::ComponentTraits

namespace arch::scene {

class Scene;

class Entity;

class Entity {
public:

	enum class Error : unsigned int {
		none = 0,
		invalidScene,
		invalidHandle,
		nullHandleID,
		nullHandleVersion,
		invalidHierarchy,
		differentScene,
		componentNotFound,
		newParentIsDescendant,
		newChildIsAncestor,
		noParent,
		noChildren,
		noDescendants
	};

	Entity() noexcept = default;
	Entity(Scene& scene, const ecs::Entity entity) noexcept;
	explicit Entity(Scene& scene) noexcept;
	// przerobić tworzenie entity -> _configureNewEntity()
	// Entity(Entity& parent);
	Entity(const Entity&) noexcept = default;
	Entity(Entity&&) noexcept = default;

	Entity& operator=(const Entity&) noexcept = default;
	Entity& operator=(Entity&&) noexcept = default;
	Entity& operator=(const ecs::Entity entity) noexcept;

	void swap(Entity& other) noexcept;

	ecs::_details::EntityTraits::IdT id() const noexcept;
	ecs::_details::EntityTraits::VersionT version() const noexcept;

	bool hasTag(std::string_view tag) const;
	Res<bool, Error> hasTagOpt(std::string_view tag) const noexcept;

	bool addTag(std::string_view tag);
	Res<bool, Error> addTagOpt(std::string_view tag) noexcept;

	bool removeTag(std::string_view tag);
	Res<bool, Error> removeTagOpt(std::string_view tag) noexcept;

	void untag();
	Res<void, Error> untagOpt() noexcept;

	template<class C>
	using GetResult = std::conditional_t<ARCH_CTRAITS<C>::flag, bool, C&>;
	template<class C>
	using GetResultOpt = std::conditional_t<ARCH_CTRAITS<C>::flag, Res<bool, Error>, RefRes<C, Error>>;

	template<class C, class... Args>
	GetResult<C> addComponent(Args&&... args);
	template<class C, class... Args>
	GetResultOpt<C> addComponentOpt(Args&&... args) noexcept;

	template<class C>
	requires(!std::is_const_v<C> && !ARCH_CTRAITS<C>::flag)
	GetResult<C> getComponent();
	template<class C>
	GetResult<const C> getComponent() const;
	template<class C>
	requires(!std::is_const_v<C> && !ARCH_CTRAITS<C>::flag)
	GetResultOpt<C> getComponentOpt() noexcept;
	template<class C>
	GetResultOpt<const C> getComponentOpt() const noexcept;

	template<class C>
	bool removeComponent();
	template<class C>
	requires(ARCH_CTRAITS<C>::movable && !ARCH_CTRAITS<C>::flag)
	C removeComponent(MoveFlag);
	template<class C>
	Res<bool, Error> removeComponentOpt() noexcept;
	template<class C>
	requires(ARCH_CTRAITS<C>::movable && !ARCH_CTRAITS<C>::flag)
	MoveRes<C, Error> removeComponentOpt(MoveFlag) noexcept;

	template<class C>
	bool hasComponent() const;
	template<class C>
	Res<bool, Error> hasComponentOpt() const noexcept;

	operator ecs::Entity() const noexcept;
	ecs::Entity handle() const noexcept;

	bool valid() const noexcept;
	operator bool() const noexcept;

	bool operator==(const Entity&) const noexcept = default;

	bool setParent(Entity& newParent);
	Res<void, Error> setParentOpt(Entity& newParent) noexcept;

	Entity addChild();
	Res<Entity, Error> addChildOpt() noexcept;

	bool isChildOf(const Entity& possibleParent) const;
	Res<bool, Error> isChildOfOpt(const Entity& possibleParent) const noexcept;
	bool hasChild(const Entity& possibleChild) const;
	Res<bool, Error> hasChildOpt(const Entity& possibleChild) const noexcept;

	bool isDescendantOf(const Entity& possibleAncestor) const;
	Res<bool, Error> isDescendantOfOpt(const Entity& possibleAncestor) const noexcept;

	Entity nthAncestor(u32 n = 1) const;
	Res<Entity, Error> nthAncestorOpt(u32 n = 1) const noexcept;
	i64 ancestry(const Entity& other) const;
	Res<i64, Error> ancestryOpt(const Entity& other) const noexcept;

	u32 depth() const;
	Res<u32, Error> depthOpt() const noexcept;

	Entity parent() const;
	Res<Entity, Error> parentOpt() const noexcept;

	Entity nthChild(u32 n = 1) const;
	Res<Entity, Error> nthChildOpt(u32 n = 1) const noexcept;
	Entity firstChild() const;
	Res<Entity, Error> firstChildOpt() const noexcept;
	Entity lastChild() const;
	Res<Entity, Error> lastChildOpt() const noexcept;
	u32 childrenCount() const;
	Res<u32, Error> childrenCountOpt() const noexcept;

	BoundedRange<ChildrenIterator> children();
	Res<BoundedRange<ChildrenIterator>, Error> childrenOpt() noexcept;

	BoundedRange<HierarchyIterator> descendants();
	Res<BoundedRange<HierarchyIterator>, Error> descendantsOpt() noexcept;

	NameComponent& getName();
	const NameComponent& getName() const;
	RefRes<NameComponent, Error> getNameOpt() noexcept;
	RefRes<const NameComponent, Error> getNameOpt() const noexcept;

	const UUID& getUUID() const;
	RefRes<const UUID, Error> getUUIDOpt() const noexcept;

	Error error() const noexcept;

private:

	friend class Scene;
	friend struct EntityProxyMaker;
	friend class ChildrenIterator;
	friend class HierarchyIterator;

	// Entity from node (valid)
	Entity(Scene& scene, hier::HierarchyNode& node) noexcept;
	// Entity from scene pointer (invalid)
	Entity(Scene* scene) noexcept;

	// throws if invalid
	void _assertValid() const;
	// throws if this invalid, other invalid or on scene difference
	void _assertValid(const Entity& other) const;
	// gets error equivalent to exception from _assertValid(other)
	Error _getError(const Entity& other) const noexcept;

	Scene* _scene = nullptr;
	ecs::Entity _entity = ecs::nullEntity;
	hier::HierarchyNode* _node = nullptr;
};

} // namespace arch::scene

#include "Entity.hpp"

#undef ARCH_CTRAITS
