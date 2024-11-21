#include <Scene.h>

namespace arch {

Scene::Scene() noexcept: _domain{} {
	const auto root = _domain.newEntity();
	_rootNode = &_domain.addComponent<Node>(root, root);
}

ecs::Entity Scene::newEntity() noexcept {
	const auto newEntity = _domain.newEntity();
	auto& newNode = _domain.addComponent<Node>(newEntity, newEntity);

	newNode._setParentUnchecked(*_rootNode);

	return newEntity;
}

void Scene::removeEntity(const ecs::Entity entity) noexcept {
	auto& node = _domain.getComponent<hier::HierarchyNode>(entity);
	node._unparent();

	_domain.kill(node._entity);
}

ecs::Domain& Scene::domain() noexcept {
	return _domain;
}

const ecs::Domain& Scene::domain() const noexcept {
	return _domain;
}

ecs::Entity Scene::root() const noexcept {
	return _rootNode->_entity;
}

Scene::Node& Scene::rootNode() noexcept {
	return *_rootNode;
}

const Scene::Node& Scene::rootNode() const noexcept {
	return *_rootNode;
}

} // namespace arch
