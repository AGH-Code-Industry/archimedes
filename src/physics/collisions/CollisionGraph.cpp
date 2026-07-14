#include <archimedes/physics/collisions/CollisionGraph.h>

namespace arch::physics {
OptRef<const std::unordered_map<ecs::Entity, Collision>> CollisionGraph::getCollisions(ecs::Entity entity) const {
	if (auto collisionsIterator = _graph.find(entity); collisionsIterator != _graph.end()) {
		return collisionsIterator->second;
	}
	return std::nullopt;
}

std::vector<ecs::Entity> CollisionGraph::getCollidingEntities() const {
	std::vector<ecs::Entity> result;
	result.reserve(_graph.size());
	for (auto& [entity, _] : _graph) {
		result.push_back(entity);
	}
	return result;
}

void CollisionGraph::addCollision(ecs::Entity entity1, ecs::Entity entity2, const Collision& collision) {
	_graph.try_emplace(entity1);
	_graph[entity1].try_emplace(entity2, collision);
}

void CollisionGraph::updateCollision(ecs::Entity entity1, ecs::Entity entity2, const Collision& collision) {
	auto collisionsIterator = _graph.find(entity1);
	if (collisionsIterator == _graph.end()) {
		return;
	}
	std::unordered_map<ecs::Entity, Collision>& collisions = collisionsIterator->second;
	auto singleCollisionIterator = collisions.find(entity2);
	if (singleCollisionIterator == collisions.end()) {
		return;
	}
	singleCollisionIterator->second = collision;
}

void CollisionGraph::removeCollision(ecs::Entity entity1, ecs::Entity entity2) {
	auto collisionsIterator = _graph.find(entity1);
	if (collisionsIterator == _graph.end()) {
		return;
	}
	collisionsIterator->second.erase(entity2);
	if (collisionsIterator->second.empty()) {
		_graph.erase(entity1);
	}
}

std::optional<Collision> CollisionGraph::getCollision(ecs::Entity entity1, ecs::Entity entity2) const {
	auto collisionsIterator = _graph.find(entity1);
	if (collisionsIterator == _graph.end()) {
		return std::nullopt;
	}
	const std::unordered_map<ecs::Entity, Collision>& collisions = collisionsIterator->second;
	auto singleCollisionIterator = collisions.find(entity2);
	if (singleCollisionIterator == collisions.end()) {
		return std::nullopt;
	}
	return singleCollisionIterator->second;
}

} // namespace arch::physics
