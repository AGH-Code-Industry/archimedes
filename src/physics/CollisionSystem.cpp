#include <archimedes/physics/CollisionSystem.h>


namespace arch::physics {

CollisionSystem::CollisionSystem(ecs::Domain& domain): _domain(domain) {}

std::vector<ecs::Entity> CollisionSystem::getEnteredCollisions(ecs::Entity entity) const {
    std::vector<ecs::Entity> result;
    if (_enteredCollisions.contains(entity)) {
        result.insert(result.end(), _enteredCollisions.at(entity).begin(), _enteredCollisions.at(entity).end());
    }
    return result;
}

std::vector<ecs::Entity> CollisionSystem::getExitedCollisions(ecs::Entity entity) const {
    std::vector<ecs::Entity> result;
    if (_exitedCollisions.contains(entity)) {
        result.insert(result.end(), _exitedCollisions.at(entity).begin(), _exitedCollisions.at(entity).end());
    }
    return result;
}

std::vector<ecs::Entity> CollisionSystem::getCurrentCollisions(ecs::Entity entity) const {
    std::vector<ecs::Entity> result;
    if (_currentCollisions.contains(entity)) {
        result.insert(result.end(), _currentCollisions.at(entity).begin(), _currentCollisions.at(entity).end());
    }
    return result;
}

CollisionMap CollisionSystem::_getCollidableEntities() const {
    std::vector<ecs::Entity> entities;
    auto view = _domain.view<ColliderComponent>();
    for (auto entity : view) {
        if(_domain.hasComponent<TransformComponent>(entity)) {
            entities.push_back(entity);
        }
    }
    CollisionMap collisions;
    for(i32 i=0; i<entities.size(); i++){
        for(i32 j=i+1; j<entities.size(); j++){
            ecs::Entity entity1 = entities[i];
            ecs::Entity entity2 = entities[j];
            if(!ColliderComponent::areColliding(
                _domain.getComponent<ColliderComponent>(entity1),
                _domain.getComponent<ColliderComponent>(entity2),
                _domain.getComponent<TransformComponent>(entity1),
                _domain.getComponent<TransformComponent>(entity2)
            )) {
                continue;
            }
            collisions.insert({entity1, entity2});
            collisions.insert({entity2, entity1});
        }
    }
    return collisions;
}

void CollisionSystem::_removeStaleCollisions(const CollisionSet& newCollisions) {
    for (const auto& [entity1, entity2] : _exitedCollisions) {
        if (!newCollisions.contains({entity1, entity2})) {
            _removeCollision(entity1, entity2);
        }
    }
}

void CollisionSystem::_saveExitedCollisions(const CollisionSet& newCollisions) {
    for (const auto& [entity1, entity2] : _lastingCollisions) {
        if (!newCollisions.contains({entity1, entity2})) {
            _moveCollision(entity1, entity2, CollisionState::Lasting, CollisionState::Exited);
        }
    }
    for (const auto& [entity1, entity2] : _enteredCollisions) {
        if (!newCollisions.contains({entity1, entity2})) {
            _moveCollision(entity1, entity2, CollisionState::Entered, CollisionState::Exited);
        }
    }
}

void CollisionSystem::_readCurrentCollisions(const CollisionSet& newCollisions) {
    for (const auto& [entity1, entity2] : newCollisions) {
        CollisionState state = _getCollisionState(entity1, entity2);
        switch(state){
            case CollisionState::Entered:
                _moveCollision(entity1, entity2, CollisionState::Entered, CollisionState::Lasting);
                break;
            case CollisionState::Exited:
                _moveCollision(entity1, entity2, CollisionState::Exited, CollisionState::Entered);
                break;
            case CollisionState::NotExisting:
                _addCollision(entity1, entity2, CollisionState::Entered);
                break;
            case CollisionState::Lasting:
                break;
        }
    }
}

void CollisionSystem::update() {
    CollisionMap collidableEntities = _getCollidableEntities();
    _removeStaleCollisions(collidableEntities);
    _saveExitedCollisions(collidableEntities);
    _readCurrentCollisions(collidableEntities);
}

}