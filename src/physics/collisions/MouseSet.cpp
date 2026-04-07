#include <archimedes/physics/collisions/MouseSet.h>

namespace arch::physics {

void MouseSet::addCollision(ecs::Entity entity, CollisionState state) {
    if(!_set.contains(entity)) {
         _set[entity] = state;
    }
}

void MouseSet::updateCollision(ecs::Entity entity, CollisionState toState) {
    if(_set.contains(entity)) {
         _set[entity] = toState;
    }
}

void MouseSet::removeCollision(ecs::Entity entity) {
    if(_set.contains(entity)) {
         _set.erase(entity);
    }
}

std::vector<ecs::Entity> MouseSet::getMouseEntities() const {
    std::vector<ecs::Entity> result;
    for(auto& [entity, _] : _set) {
        result.push_back(entity);
    }
    return result;
}

std::optional<CollisionState> MouseSet::getCollision(ecs::Entity entity) const {
    if(_set.contains(entity)) {
        return _set.at(entity);
    }
	return std::nullopt;
}

} // namespace arch::physics