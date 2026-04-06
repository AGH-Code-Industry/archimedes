#include <archimedes/physics/collisions/CollisionSystem.h>
#include <archimedes/physics/collisions/ColliderComponent.h>
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

std::vector<ecs::Entity> CollisionSystem::getCollisions(ecs::Entity entity) const {
    std::vector<ecs::Entity> result;
    for(auto& [otherEntity, state] : _savedCollisions.getCollisions(entity)) {
        if(state == CollisionState::Lasting || state == CollisionState::Entered) {
            result.push_back(otherEntity);
        }
    }
    return result;
}

CollisionGraph CollisionSystem::_getCollidedEntities() const {
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

MouseSet CollisionSystem::_getMouseEntities(float3 mousePosition) const {
    MouseSet mouseSet;
    auto view = _domain.view<ColliderComponent>();
    for (auto entity : view) {
        if(!_domain.hasComponent<TransformComponent>(entity)) {
            continue;        
        }
        auto& collider = _domain.getComponent<ColliderComponent>(entity);
        auto& transform = _domain.getComponent<TransformComponent>(entity);
        if(!ColliderComponent::isCollidingWithMouse(
            collider,
            transform,
            mousePosition
        )) {
            continue;
        }
            mouseSet.addCollision(entity, CollisionState::CurrentlyFound);
        }
    return mouseSet;
}

void CollisionSystem::_checkDisappearedMouseCollisions(const MouseSet& newMouseCollisions) {
    for(auto& entity : _mouseCollisions.getMouseEntities()) {
        CollisionState state = _mouseCollisions.getCollisionState(entity);
        if(newMouseCollisions.getCollisionState(entity) == CollisionState::NotExisting) {
            if(state == CollisionState::Entered || state == CollisionState::Lasting) {
                _mouseCollisions.changeCollisionState(entity, CollisionState::Exited);
            } else if(state == CollisionState::Exited) {
                _mouseCollisions.removeCollision(entity);
            }
        }
    }
}

void CollisionSystem::_readCurrentMouseCollisions(const MouseSet& newMouseCollisions) {
    for(auto& entity : newMouseCollisions.getMouseEntities()) {
        CollisionState state = _mouseCollisions.getCollisionState(entity);
        switch(state){
            case CollisionState::Entered:
                _mouseCollisions.changeCollisionState(entity, CollisionState::Lasting);
                break;
            case CollisionState::Exited:
                _mouseCollisions.changeCollisionState(entity, CollisionState::Entered);
                break;
            case CollisionState::NotExisting:
                _mouseCollisions.addCollision(entity, CollisionState::Entered);
                break;
            case CollisionState::Lasting:
                break;
            case CollisionState::CurrentlyFound:
                throw PhysicsException("Saved mouse collision's state should not be CurrentlyFound");
        }
    }
}

bool CollisionSystem::hasMouseEntered(ecs::Entity entity) const {
    return _mouseCollisions.getCollisionState(entity) == CollisionState::Entered;
}

bool CollisionSystem::hasMouse(ecs::Entity entity) const {
    CollisionState state = _mouseCollisions.getCollisionState(entity);
    return state == CollisionState::Entered || state == CollisionState::Lasting;
}

bool CollisionSystem::hasMouseExited(ecs::Entity entity) const {
    return _mouseCollisions.getCollisionState(entity) == CollisionState::Exited;
}

void CollisionSystem::update(float3 mousePosition) {
    CollisionGraph collidedEntities = _getCollidedEntities();
    _checkDisappearedCollisions(collidedEntities);
    _readCurrentCollisions(collidedEntities);
    MouseSet mouseEntities = _getMouseEntities(mousePosition);
    _checkDisappearedMouseCollisions(mouseEntities);
    _readCurrentMouseCollisions(mouseEntities);
}

}