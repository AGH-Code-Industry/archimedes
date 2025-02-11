#include <Scene.h>
#include <scene/Entity.h>
#include <scene/TagDB.h>
#include <scene/Tags.h>

namespace arch::scene {

Scene::Scene() noexcept: _domain{} {
	const auto root = _domain.newEntity();
	_rootNode = &_domain.addComponent<Node>(root, root);
}

Entity Scene::newEntity() noexcept {
	Entity result;

	result._entity = _domain.newEntity();
	result._node = &_domain.addComponent<Node>(result._entity, result._entity);
	result._node->_setParentUnchecked(*_rootNode);
	result._scene = this;

	return result;
}

ecs::Entity Scene::newEntity(ReturnHandleFlag) noexcept {
	const auto newEntity = _domain.newEntity();
	auto& newNode = _domain.addComponent<Node>(newEntity, newEntity);

	newNode._setParentUnchecked(*_rootNode);

	return newEntity;
}

void Scene::removeEntity(Entity& entity) noexcept {
	auto& node = _domain.getComponent<hier::HierarchyNode>(entity);
	node._unparent();

	untag(entity);

	_domain.kill(node._entity);

	entity._node = nullptr;
}

void Scene::removeEntity(const ecs::Entity entity) noexcept {
	auto& node = _domain.getComponent<hier::HierarchyNode>(entity);
	node._unparent();

	untag(entity);

	_domain.kill(node._entity);
}

ecs::Domain& Scene::domain() noexcept {
	return _domain;
}

const ecs::Domain& Scene::domain() const noexcept {
	return _domain;
}

Entity Scene::root() noexcept {
	return Entity(*this, *_rootNode);
}

ecs::Entity Scene::root(ReturnHandleFlag) const noexcept {
	return _rootNode->entity();
}

Scene::Node& Scene::rootNode() noexcept {
	return *_rootNode;
}

const Scene::Node& Scene::rootNode() const noexcept {
	return *_rootNode;
}

bool Scene::addTag(const ecs::Entity entity, std::string_view tag) noexcept {
	auto& entitiesOfTag = _domain.global<TagDB>().value;
	auto entitiesSetIt = entitiesOfTag.find(tag);
	if (entitiesSetIt == entitiesOfTag.end()) {
		auto&& [it, ignored] = entitiesOfTag.insert({ std::string(tag), {} });
		entitiesSetIt = it;
	}
	auto&& [it, inserted] = entitiesSetIt->second.insert(entity);

	if (inserted) { // add tag to entity
		_domain.addComponent<Tags>(entity).value.insert(tag);
		return true;
	}
	return false;
}

bool Scene::removeTag(const ecs::Entity entity, std::string_view tag) noexcept {
	auto tagsOpt = _domain.tryGetComponent<Tags>(entity);
	if (!tagsOpt) {
		return false;
	}
	auto& tags = tagsOpt->value;
	auto found = tags.find(tag);
	if (found != tags.end()) { // has tag
		tags.extract(tag);
		_domain.global<TagDB>().value.find(tag)->second.extract(entity);
		return true;
	}
	return false;
}

bool Scene::hasTag(const ecs::Entity entity, std::string_view tag) const noexcept {
	auto tagsOpt = _domain.tryGetComponent<Tags>(entity);
	return tagsOpt ? tagsOpt->value.find(tag) != tagsOpt->value.end() : false;
}

void Scene::untag(const ecs::Entity entity) noexcept {
	auto tagsOpt = _domain.tryGetComponent<Tags>(entity);
	if (tagsOpt) {
		auto& entitiesOfTag = _domain.global<TagDB>().value;
		for (auto&& tag : tagsOpt->value) {
			auto found = entitiesOfTag.find(tag);
			found->second.extract(entity);
		}
	}
}

OptRef<const std::unordered_set<std::string_view>> Scene::tagsOf(const ecs::Entity entity) const noexcept {
	auto tagsOpt = _domain.tryGetComponent<Tags>(entity);
	return tagsOpt ? OptRef<const std::unordered_set<std::string_view>>(tagsOpt->value) : std::nullopt;
}

} // namespace arch::scene
