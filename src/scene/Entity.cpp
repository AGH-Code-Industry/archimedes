#include <scene/ChildrenIterator.h>
#include <scene/Entity.h>
#include <utils/Res.h>

namespace arch::scene {

Entity::Error Entity::error() const noexcept {
	if (!_scene) {
		return Error::invalidScene;
	} else if (ecs::_details::EntityTraits::Id::hasNull(_entity)) {
		return Error::nullHandleID;
	} else if (ecs::_details::EntityTraits::Version::hasNull(_entity)) {
		return Error::nullHandleVersion;
	} else if (!_scene->domain().alive(_entity)) {
		return Error::invalidHandle;
	} else if (!_node) {
		return Error::invalidHierarchy;
	} else {
		return Error::none;
	}
}

Entity::Error Entity::_getError(const Entity& other) const noexcept {
	const auto errThis = error();
	if (errThis != Error::none) {
		return errThis;
	}

	const auto errOther = other.error();
	if (errOther != Error::none) {
		return errOther;
	}

	return _scene == other._scene ? Error::none : Error::differentScene;
}

void Entity::_assertValid() const {
	switch (error()) {
		case Error::invalidScene:	   throw InvalidEntityException("Entity has no scene");
		case Error::nullHandleID:	   throw InvalidEntityException("Entity has null ID part");
		case Error::nullHandleVersion: throw InvalidEntityException("Entity has null Version part");
		case Error::invalidHandle:	   throw InvalidEntityException("Entity has invalid handle");
		case Error::invalidHierarchy:  throw InvalidEntityException("Entity is not in hierarchy");
	}
}

void Entity::_assertValid(const Entity& other) const {
	_assertValid();
	other._assertValid();
	if (_getError(other) != Error::none) {
		throw InvalidEntityException("Other Entity is in different Scene");
	}
}

Entity::Entity(Scene& scene, const ecs::Entity entity) noexcept:
	_scene{ &scene },
	_entity{ entity },
	_node{ [&scene, entity]() {
		auto opt = scene.domain().tryGetComponent<hier::HierarchyNode>(entity);
		return opt ? &*opt : nullptr;
	}() } {
	//
}

Entity::Entity(Scene* scene) noexcept: _scene{ scene } {}

Entity::Entity(Scene& scene) noexcept:
	_scene{ &scene },
	_entity{ _scene->_newEntity().entity() },
	_node{ &_scene->domain().getComponent<hier::HierarchyNode>(_entity) } {
	//
}

// Entity::Entity(Entity& parent): Entity((parent._assertValid(), *parent._scene)) {
//	_node->setParent(*parent._node);
// }

Entity::Entity(Scene& scene, hier::HierarchyNode& node) noexcept:
	_scene{ &scene },
	_node{ &node },
	_entity{ node.entity() } {}

Entity& Entity::operator=(const ecs::Entity entity) noexcept {
	return *this = Entity(*_scene, entity);
}

void Entity::swap(Entity& other) noexcept {
	std::swap(_scene, other._scene);
	std::swap(_entity, other._entity);
	std::swap(_node, other._node);
}

ecs::_details::EntityTraits::IdT Entity::id() const noexcept {
	return *_entity;
}

ecs::_details::EntityTraits::VersionT Entity::version() const noexcept {
	return ~_entity;
}

bool Entity::hasTag(std::string_view tag) const {
	_assertValid();
	return _scene->hasTag(_entity, tag);
}

Res<bool, Entity::Error> Entity::hasTagOpt(std::string_view tag) const noexcept {
	auto err = error();
	if (err == Error::none) {
		return _scene->hasTag(_entity, tag);
	} else {
		return Err(err);
	}
}

bool Entity::addTag(std::string_view tag) {
	_assertValid();
	return _scene->addTag(_entity, tag);
}

Res<bool, Entity::Error> Entity::addTagOpt(std::string_view tag) noexcept {
	auto err = error();
	if (err == Error::none) {
		return _scene->addTag(_entity, tag);
	} else {
		return Err(err);
	}
}

bool Entity::removeTag(std::string_view tag) {
	_assertValid();
	return _scene->removeTag(_entity, tag);
}

Res<bool, Entity::Error> Entity::removeTagOpt(std::string_view tag) noexcept {
	auto err = error();
	if (err == Error::none) {
		return _scene->removeTag(_entity, tag);
	} else {
		return Err(err);
	}
}

void Entity::untag() {
	_assertValid();
	return _scene->untag(_entity);
}

Res<void, Entity::Error> Entity::untagOpt() noexcept {
	auto err = error();
	if (err == Error::none) {
		_scene->untag(_entity);
		return {};
	} else {
		return Err(err);
	}
}

Entity::operator ecs::Entity() const noexcept {
	return _entity;
}

ecs::Entity Entity::handle() const noexcept {
	return _entity;
}

bool Entity::valid() const noexcept {
	return error() == Error::none;
}

Entity::operator bool() const noexcept {
	return valid();
}

bool Entity::setParent(Entity& newParent) {
	_assertValid(newParent);
	return _node->setParent(*newParent._node);
}

Res<void, Entity::Error> Entity::setParentOpt(Entity& newParent) noexcept {
	auto err = _getError(newParent);
	if (err != Error::none) {
		return Err(err);
	}

	if (_node->setParent(*newParent._node)) {
		return {};
	} else {
		return Err(Error::newParentIsDescendant);
	}
}

Entity Entity::addChild() {
	_assertValid();
	auto newEntity = _scene->newEntity();
	newEntity.setParent(*this);
	return newEntity;
}

Res<Entity, Entity::Error> Entity::addChildOpt() noexcept {
	auto err = error();
	if (err == Error::none) {
		auto newEntity = _scene->newEntity();
		auto res = newEntity.setParentOpt(*this);
		if (res) {
			return newEntity;
		} else {
			return Err(res.error());
		}
	} else {
		return Err(err);
	}
}

bool Entity::isChildOf(const Entity& possibleParent) const {
	_assertValid(possibleParent);
	return _node->isChildOf(*possibleParent._node);
}

Res<bool, Entity::Error> Entity::isChildOfOpt(const Entity& possibleParent) const noexcept {
	auto err = _getError(possibleParent);
	if (err != Error::none) {
		return Err(err);
	}
	return _node->isChildOf(*possibleParent._node);
}

bool Entity::hasChild(const Entity& possibleChild) const {
	return possibleChild.isChildOf(*this);
}

Res<bool, Entity::Error> Entity::hasChildOpt(const Entity& possibleChild) const noexcept {
	return possibleChild.isChildOfOpt(*this);
}

bool Entity::isDescendantOf(const Entity& possibleAncestor) const {
	_assertValid(possibleAncestor);
	return _node->ancestry(*possibleAncestor._node) > 0;
}

Res<bool, Entity::Error> Entity::isDescendantOfOpt(const Entity& possibleAncestor) const noexcept {
	auto err = _getError(possibleAncestor);
	if (err != Error::none) {
		return Err(err);
	}
	return _node->ancestry(*possibleAncestor._node) > 0;
}

Entity Entity::nthAncestor(u32 n) const {
	_assertValid();
	return Entity(*_scene, _node->ancestor(n));
}

Res<Entity, Entity::Error> Entity::nthAncestorOpt(u32 n) const noexcept {
	auto err = error();
	if (err == Error::none) {
		return Entity(*_scene, _node->ancestor(n));
	} else {
		return Err(err);
	}
}

i64 Entity::ancestry(const Entity& other) const {
	_assertValid(other);
	return _node->ancestry(*other._node);
}

Res<i64, Entity::Error> Entity::ancestryOpt(const Entity& other) const noexcept {
	auto err = _getError(other);
	if (err == Error::none) {
		return _node->ancestry(*other._node);
	} else {
		return Err(err);
	}
}

u32 Entity::depth() const {
	_assertValid();
	return _node->depth();
}

Res<u32, Entity::Error> Entity::depthOpt() const noexcept {
	auto err = error();
	if (err == Error::none) {
		return _node->depth();
	} else {
		return Err(err);
	}
}

Entity Entity::parent() const {
	_assertValid();
	auto parentPtr = &_node->parent();
	if (parentPtr == nullptr || parentPtr->isRoot()) {
		throw InvalidEntityException("There is no parent");
	} else {
		return Entity(*_scene, *parentPtr);
	}
}

Res<Entity, Entity::Error> Entity::parentOpt() const noexcept {
	auto err = error();
	if (err == Error::none) {
		auto parentPtr = &_node->parent();
		if (parentPtr == nullptr || parentPtr->isRoot()) {
			return Err(Error::noParent);
		} else {
			return Entity(*_scene, *parentPtr);
		}
	} else {
		return Err(err);
	}
}

Entity Entity::nthChild(u32 n) const {
	_assertValid();
	auto&& nthOpt = _node->childOpt(n);
	if (nthOpt) {
		return Entity(*_scene, nthOpt.get());
	} else {
		return Entity(_scene);
	}
}

Res<Entity, Entity::Error> Entity::nthChildOpt(u32 n) const noexcept {
	auto err = error();
	if (err == Error::none) {
		auto&& nthOpt = _node->childOpt(n);
		if (nthOpt) {
			return Entity(*_scene, nthOpt.get());
		} else {
			return Err(Error::noChildren);
		}
	} else {
		return Err(err);
	}
}

Entity Entity::firstChild() const {
	_assertValid();
	auto&& nthOpt = _node->firstChildOpt();
	if (nthOpt) {
		return Entity(*_scene, nthOpt.get());
	} else {
		return Entity(_scene);
	}
}

Res<Entity, Entity::Error> Entity::firstChildOpt() const noexcept {
	auto err = error();
	if (err == Error::none) {
		auto&& nthOpt = _node->firstChildOpt();
		if (nthOpt) {
			return Entity(*_scene, nthOpt.get());
		} else {
			return Err(Error::noChildren);
		}
	} else {
		return Err(err);
	}
}

Entity Entity::lastChild() const {
	_assertValid();
	auto&& nthOpt = _node->lastChildOpt();
	if (nthOpt) {
		return Entity(*_scene, nthOpt.get());
	} else {
		return Entity(_scene);
	}
}

Res<Entity, Entity::Error> Entity::lastChildOpt() const noexcept {
	auto err = error();
	if (err == Error::none) {
		auto&& nthOpt = _node->lastChildOpt();
		if (nthOpt) {
			return Entity(*_scene, nthOpt.get());
		} else {
			return Err(Error::noChildren);
		}
	} else {
		return Err(err);
	}
}

u32 Entity::childrenCount() const {
	_assertValid();
	return _node->childrenCount();
}

Res<u32, Entity::Error> Entity::childrenCountOpt() const noexcept {
	auto err = error();
	if (err == Error::none) {
		return _node->childrenCount();
	} else {
		return Err(err);
	}
}

BoundedRange<ChildrenIterator> Entity::children() {
	_assertValid();
	if (_node->childrenCount() == 0) {
		throw InvalidEntityException("Entity has no children");
	} else {
		return BoundedRange(ChildrenIterator(_node->begin(), this), ChildrenIterator(_node->end(), this));
	}
}

Res<BoundedRange<ChildrenIterator>, Entity::Error> Entity::childrenOpt() noexcept {
	auto err = error();
	if (err == Error::none) {
		return BoundedRange(ChildrenIterator(_node->begin(), this), ChildrenIterator(_node->end(), this));
	} else {
		return Err(err);
	}
}

BoundedRange<HierarchyIterator> Entity::descendants() {
	_assertValid();
	if (_node->childrenCount() == 0) {
		throw InvalidEntityException("Entity has no children");
	} else {
		auto iterable = _node->recursiveIterable();
		return BoundedRange(HierarchyIterator(iterable.begin(), this), HierarchyIterator(iterable.end(), this));
	}
}

Res<BoundedRange<HierarchyIterator>, Entity::Error> Entity::descendantsOpt() noexcept {
	auto err = error();
	if (err == Error::none) {
		return BoundedRange(
			HierarchyIterator(_node->recursiveIterable().begin(), this),
			HierarchyIterator(_node->recursiveIterable().end(), this)
		);
	} else {
		return Err(err);
	}
}

NameComponent& Entity::getName() {
	return getComponent<NameComponent>();
}

const NameComponent& Entity::getName() const {
	return getComponent<const NameComponent>();
}

RefRes<NameComponent, Entity::Error> Entity::getNameOpt() noexcept {
	return getComponentOpt<NameComponent>();
}

RefRes<const NameComponent, Entity::Error> Entity::getNameOpt() const noexcept {
	return getComponentOpt<const NameComponent>();
}

const UUID& Entity::getUUID() const {
	return getComponent<const UUID>();
}

RefRes<const UUID, Entity::Error> Entity::getUUIDOpt() const noexcept {
	return getComponentOpt<const UUID>();
}

} // namespace arch::scene
