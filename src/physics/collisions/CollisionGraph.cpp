#include <archimedes/physics/collisions/CollisionGraph.h>


namespace arch::physics {
OptRef<const ecs::ComponentPool<Collision>> CollisionGraph::getCollisions(ecs::Entity entity) const {
	return _graph.tryGet(entity);
}

std::vector<ecs::Entity> CollisionGraph::getCollidingEntities() const {
    std::vector<ecs::Entity> result;
	for (auto node = _graph.begin(); node != _graph.end(); node++) {
        result.push_back(node.entity());
    }
    return result;
}

void CollisionGraph::addCollision(ecs::Entity entity1, ecs::Entity entity2, const Collision& collision) {
	auto currentCollisionsOpt = _graph.tryGet(entity1);
    if (!currentCollisionsOpt) {
		_graph.addComponent(entity1);
		auto& currentCollisions = _graph.get(entity1);
		currentCollisions.addComponent(entity2, collision);
		return;
    }
	currentCollisionsOpt.get().addComponent(entity2, collision);
}

void CollisionGraph::updateCollision(ecs::Entity entity1, ecs::Entity entity2, const Collision& collision) {
	auto currentCollisionsOpt = _graph.tryGet(entity1);
    if (!currentCollisionsOpt) {
		return;
    }
	auto currentCollision = currentCollisionsOpt.get().tryGet(entity2);
    if (!currentCollision) {
		return;
    }
	currentCollision.get() = collision;
}

void CollisionGraph::removeCollision(ecs::Entity entity1, ecs::Entity entity2) {
	auto currentCollisionsOpt = _graph.tryGet(entity1);
	if (!currentCollisionsOpt) {
		return;
	}
	auto& currentCollisions = currentCollisionsOpt.get();
    if (currentCollisions.contains(entity2)) {
		currentCollisions.removeComponent(entity2);
    }
    if (currentCollisions.count() == 0) {
		_graph.removeComponent(entity1);
    }
}

std::optional<Collision> CollisionGraph::getCollision(ecs::Entity entity1, ecs::Entity entity2) const {
    auto currentCollisionsOpt = _graph.tryGet(entity1);
	if (!currentCollisionsOpt) {
		return std::nullopt;
	}
	auto collisionOpt = currentCollisionsOpt.get().tryGet(entity2);
	if (!collisionOpt) {
		return std::nullopt;
	}
	return collisionOpt.get();
}

} // namespace arch::physics