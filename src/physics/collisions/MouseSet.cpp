#include <archimedes/physics/collisions/MouseSet.h>

namespace arch::physics {

void MouseSet::addCollision(ecs::Entity entity, CollisionState state) {
	_set.try_emplace(entity, state);
}

void MouseSet::updateCollision(ecs::Entity entity, CollisionState toState) {
	if (auto collisionsIterator = _set.find(entity); collisionsIterator != _set.end()) {
		collisionsIterator->second = toState;
	}
}

void MouseSet::removeCollision(ecs::Entity entity) {
	_set.erase(entity);
}

std::vector<ecs::Entity> MouseSet::getMouseEntities() const {
	std::vector<ecs::Entity> result;
	result.reserve(_set.size());
	for (auto& [entity, _] : _set) {
		result.push_back(entity);
	}
	return result;
}

std::optional<CollisionState> MouseSet::getCollision(ecs::Entity entity) const {
    if (auto it = _set.find(entity); it != _set.end()) {
        return it->second;
    }
	return std::nullopt;
}

} // namespace arch::physics
