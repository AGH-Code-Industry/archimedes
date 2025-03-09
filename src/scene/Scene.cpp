#include <Scene.h>
#include <scene/Entity.h>
#include <scene/NameComponent.h>
#include <scene/TagDB.h>
#include <scene/Tags.h>
#include <utils/UUID.h>

namespace arch::scene {

Scene::Scene() noexcept: _domain{} {
	const auto root = _domain.newEntity();
	_rootNode = &_domain.addComponent<Node>(root, root);
}

Entity Scene::newEntity() noexcept {
	Entity result;

	auto& node = _newEntity();

	result._entity = node.entity();
	result._node = &node;
	result._scene = this;

	return result;
}

hier::HierarchyNode& Scene::_newEntity() noexcept {
	const auto newEntity = _domain.newEntity();

	// Logger::debug("newEntity = {:|v}", newEntity);

	if (newEntity == ecs::_details::EntityTraits::Ent::fromParts(7, 2)) {
		// Logger::error("WTF?");
	}

	auto& newNode = _domain.addComponent<Node>(newEntity, newEntity);
	// Logger::debug("newNode = {:|v}", newNode.entity());
	_domain.addComponent<utils::UUID>(newEntity, utils::UUID::fromTime());
	_domain.addComponent<NameComponent>(newEntity, "New Entity");

	newNode._setParentUnchecked(*_rootNode);

	return newNode;
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
