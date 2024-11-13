#include <Scene.h>

namespace arch {

Scene::Node& Scene::_getNode(const ecs::Entity entity) noexcept {
	return _domain.getComponent<Node>(entity);
}

const Scene::Node& Scene::_getNode(const ecs::Entity entity) const noexcept {
	return _domain.getComponent<Node>(entity);
}

auto Scene::_tryGetNode(const ecs::Entity entity) noexcept -> decltype(_domain.tryGetComponent<Node>(entity)) {
	return _domain.tryGetComponent<Node>(entity);
}

auto Scene::_tryGetNode(const ecs::Entity entity) const noexcept -> decltype(_domain.tryGetComponent<Node>(entity)) {
	return _domain.tryGetComponent<Node>(entity);
}

void Scene::_unparent(Node& node) noexcept {
	// unlink node from siblings
	node.prev->next = node.next;
	node.next->prev = node.prev;

	if ((node.parent->count--) == 1) {
		// only child -> reset children
		node.parent->first = (node.parent->last = nullptr);
	} else if (node.prev->entity == ecs::nullEntity) {
		// first child
		node.parent->first = node.next;
	} else if (node.next->entity == ecs::nullEntity) {
		// last child
		node.parent->last = node.prev;
	}

	// unlink hierarchy
	node.next = (node.prev = (node.parent = nullptr));
	node.depth = 0;
}

void Scene::_setParentUnchecked(Node& node, Node& newParent) noexcept {
	node.parent = &newParent;

	// add at the end
	newParent.sentinel.prev = &node;
	node.next = &newParent.sentinel;

	if (newParent.count == 0) {
		// 1st child
		newParent.sentinel.next = (newParent.first = &node);
		node.prev = &newParent.sentinel;
	} else {
		node.prev = newParent.last;
		newParent.last->next = &node;
	}

	newParent.last = &node;
	++newParent.count;
	node.depth = newParent.depth + 1;
}

Scene::Scene() noexcept: _domain{} {
	const auto root = _domain.newEntity();
	_rootNode = &_domain.addComponent<Node>(root, root);
}

ecs::Entity Scene::newEntity() noexcept {
	const auto newEntity = _domain.newEntity();
	auto& newNode = _domain.addComponent<Node>(newEntity, newEntity);

	_setParentUnchecked(newNode, *_rootNode);

	return newEntity;
}

void Scene::killEntity(const ecs::Entity entity) noexcept {
	auto& node = _getNode(entity);
	_unparent(node);

	_domain.kill(node.entity);
}

bool Scene::setParent(Node& entity, Node& newParent) noexcept {
	if (ancestry(entity, newParent) > 0) {
		return false;
	}

	_unparent(entity);
	_setParentUnchecked(entity, newParent);

	return true;
}

bool Scene::setParent(const ecs::Entity entity, const ecs::Entity newParent) noexcept {
	auto entityNodeOpt = _tryGetNode(entity);
	if (entityNodeOpt) {
		auto parentNodeOpt = _tryGetNode(newParent);
		if (parentNodeOpt) {
			return setParent(entityNodeOpt->get(), parentNodeOpt->get());
		}
	}

	return false;
}

bool Scene::isChildOf(const Node& entity, const Node& parent) const noexcept {
	return entity.parent == &parent;
}

bool Scene::isChildOf(const ecs::Entity entity, const ecs::Entity parent) const noexcept {
	auto entityNodeOpt = _tryGetNode(entity);
	if (entityNodeOpt) {
		auto parentNodeOpt = _tryGetNode(parent);
		if (parentNodeOpt) {
			return isChildOf(entityNodeOpt->get(), parentNodeOpt->get());
		}
	}

	return false;
}

ecs::Entity Scene::getAncestor(const Node& entity, u32 which) const noexcept {
	const Node* node = &entity;
	while (which-- && node != _rootNode) {
		node = node->parent;
	}
	return node->entity;
}

ecs::Entity Scene::getAncestor(const ecs::Entity entity, const u32 which) const noexcept {
	auto nodeOpt = _tryGetNode(entity);
	if (nodeOpt) {
		return getAncestor(nodeOpt->get(), which);
	}
	return ecs::nullEntity;
}

i64 Scene::ancestry(const Node& ancestor, const Node& descendant) const noexcept {
	if (ancestor.depth < descendant.depth) {
		auto node = &descendant;
		const u32 steps = descendant.depth - ancestor.depth;
		for (u32 i = 0; i != steps && node; ++i) {
			node = node->parent;
		}

		return node == &ancestor ? steps : 0;
	} else if (ancestor.depth == descendant.depth) {
		return 0;
	} else {
		return -ancestry(descendant, ancestor);
	}
}

i64 Scene::ancestry(const ecs::Entity ancestor, const ecs::Entity descendant) const noexcept {
	auto ancestorNodeOpt = _tryGetNode(ancestor);
	if (ancestorNodeOpt) {
		auto descendantNodeOpt = _tryGetNode(descendant);
		if (descendantNodeOpt) {
			return ancestry(ancestorNodeOpt->get(), descendantNodeOpt->get());
		}
	}
	return 0;
}

ecs::Entity Scene::getParent(const Node& entity) const noexcept {
	return getAncestor(entity);
}

ecs::Entity Scene::getParent(const ecs::Entity entity) const noexcept {
	return getAncestor(entity);
}

ecs::Entity Scene::getChild(const Node& entity, u32 index) const noexcept {
	const hier::ChildNode* node = entity.first;
	if (node == nullptr) {
		return ecs::nullEntity;
	}

	for (; index != 0 && node->entity != ecs::nullEntity; --index) {
		node = node->next;
	}
	return node->entity;
}

ecs::Entity Scene::getChild(const ecs::Entity entity, const u32 index) const noexcept {
	auto nodeOpt = _tryGetNode(entity);
	if (nodeOpt) {
		return getChild(nodeOpt->get(), index);
	}
	return ecs::nullEntity;
}

ecs::Domain& Scene::domain() noexcept {
	return _domain;
}

const ecs::Domain& Scene::domain() const noexcept {
	return _domain;
}

ecs::Entity Scene::root() const noexcept {
	return _rootNode->entity;
}

Scene::Node& Scene::rootNode() noexcept {
	return *_rootNode;
}

const Scene::Node& Scene::rootNode() const noexcept {
	return *_rootNode;
}

} // namespace arch
