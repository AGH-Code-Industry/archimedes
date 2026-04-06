#include <archimedes/physics/CollisionSystem.h>
#include <archimedes/physics/components/ColliderComponent.h>
#include <archimedes/scene/components/TransformComponent.h>



namespace arch::physics {

using TransformComponent = arch::scene::components::TransformComponent;

CollisionSystem::CollisionSystem(ecs::Domain& domain): _domain(domain) {}

std::vector<ecs::Entity> CollisionSystem::getEnteredCollisions(ecs::Entity entity) const {
    std::vector<ecs::Entity> result;
    for(auto& [otherEntity, state] : _savedCollisions.getCollisions(entity)) {
        if(state == CollisionState::Entered) {
            result.push_back(otherEntity);
        }
    }
    return result;
}

std::vector<ecs::Entity> CollisionSystem::getExitedCollisions(ecs::Entity entity) const {
    std::vector<ecs::Entity> result;
    for(auto& [otherEntity, state] : _savedCollisions.getCollisions(entity)) {
        if(state == CollisionState::Exited) {
            result.push_back(otherEntity);
        }
    }

    return result;
}

std::vector<ecs::Entity> CollisionSystem::getLastingCollisions(ecs::Entity entity) const {
    std::vector<ecs::Entity> result;
    for(auto& [otherEntity, state] : _savedCollisions.getCollisions(entity)) {
        if(state == CollisionState::Lasting) {
            result.push_back(otherEntity);
        }
    }
    return result;
}

CollisionGraph CollisionSystem::_getCollidableEntities() const {
    std::vector<ecs::Entity> entities;
    auto view = _domain.view<ColliderComponent>();
    for (auto entity : view) {
        if(_domain.hasComponent<TransformComponent>(entity)) {
            entities.push_back(entity);
        }
    }
    CollisionGraph collisions;
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
            collisions.addCollision(entity1, entity2, CollisionState::CurrentlyFound);
            collisions.addCollision(entity2, entity1, CollisionState::CurrentlyFound);
        }
    }
    return collisions;
}

void CollisionSystem::_checkDisappearedCollisions(const CollisionGraph& newCollisions) {
    for(auto& entity1 : _savedCollisions.getCollidingEntities()) {
        for(auto& [entity2, state] : _savedCollisions.getCollisions(entity1)) {
            if(newCollisions.getCollisionState(entity1, entity2) == CollisionState::NotExisting) {
                if(state == CollisionState::Entered || state == CollisionState::Lasting) {
                    _savedCollisions.changeCollisionState(entity1, entity2, CollisionState::Exited);
                } else if(state == CollisionState::Exited) {
                    _savedCollisions.removeCollision(entity1, entity2);
                }
            }
        }
    }
}

void CollisionSystem::_readCurrentCollisions(const CollisionGraph& newCollisions) {
    for(auto& entity1 : newCollisions.getCollidingEntities()) {
        for(auto& [entity2, _] : newCollisions.getCollisions(entity1)) {
            CollisionState state = _savedCollisions.getCollisionState(entity1, entity2);
            switch(state){
                case CollisionState::Entered:
                    _savedCollisions.changeCollisionState(entity1, entity2, CollisionState::Lasting);
                    break;
                case CollisionState::Exited:
                    _savedCollisions.changeCollisionState(entity1, entity2, CollisionState::Entered);
                    break;
                case CollisionState::NotExisting:
                    _savedCollisions.addCollision(entity1, entity2, CollisionState::Entered);
                    break;
                case CollisionState::Lasting:
                    break;
                case CollisionState::CurrentlyFound:
                    throw PhysicsException("Saved collision's state should not be CurrentlyFound");
            }
        }
    }
}

void CollisionSystem::update() {
    CollisionGraph collidableEntities = _getCollidableEntities();
    _checkDisappearedCollisions(collidableEntities);
    _readCurrentCollisions(collidableEntities);
}

}