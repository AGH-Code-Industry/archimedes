#include <archimedes/physics/collisions/CollisionSystem.h>
#include <archimedes/physics/collisions/ColliderComponent.h>
#include <archimedes/scene/components/TransformComponent.h>



namespace arch::physics {

using TransformComponent = arch::scene::components::TransformComponent;

CollisionSystem::CollisionSystem(ecs::Domain& domain): _domain(domain) {}

std::unordered_map<ecs::Entity, Collision> CollisionSystem::getEnteredCollisions(ecs::Entity entity) const {
    std::unordered_map<ecs::Entity, Collision> result;
    for(auto& [otherEntity, collision] : _savedCollisions.getCollisions(entity)) {
        if(collision.state == CollisionState::Entered) {
			result[otherEntity] = collision;
        }
    }
    return result;
}

std::unordered_map<ecs::Entity, Collision> CollisionSystem::getExitedCollisions(ecs::Entity entity) const {
	std::unordered_map<ecs::Entity, Collision> result;
    for(auto& [otherEntity, collision] : _savedCollisions.getCollisions(entity)) {
        if(collision.state == CollisionState::Exited) {
			result[otherEntity] = collision;
        }
    }

    return result;
}

std::unordered_map<ecs::Entity, Collision> CollisionSystem::getCollisions(ecs::Entity entity) const {
	std::unordered_map<ecs::Entity, Collision> result;
    for(auto& [otherEntity, collision] : _savedCollisions.getCollisions(entity)) {
        if(collision.state == CollisionState::Lasting || collision.state == CollisionState::Entered) {
			result[otherEntity] = collision;
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
			auto& collider1 = _domain.getComponent<ColliderComponent>(entities[i]);
			auto& collider2 = _domain.getComponent<ColliderComponent>(entities[j]);
			bool isSecondScanned = (collider1.scansMask & collider2.isScannedMask).any();
			bool isFirstScanned = (collider2.scansMask & collider1.isScannedMask).any();
			if (not isSecondScanned and not isFirstScanned) {
				continue;
			}
            ecs::Entity entity1 = entities[i];
            ecs::Entity entity2 = entities[j];
			std::optional<Collision> collision = ColliderComponent::areColliding(
				collider1,
                collider2,
				_domain.getComponent<TransformComponent>(entity1),
				_domain.getComponent<TransformComponent>(entity2)
			);
            if (!collision) {
				continue;
            }
            if (isSecondScanned) {
				collisions.addCollision(entity1, entity2, collision.value());
			} else {
				collisions.addCollision(entity2, entity1, collision.value());			    
            }
        }
    }
    return collisions;
}

void CollisionSystem::_checkDisappearedCollisions(const CollisionGraph& newCollisions) {
    for(auto& entity1 : _savedCollisions.getCollidingEntities()) {
        for(auto& [entity2, savedCollision] : _savedCollisions.getCollisions(entity1)) {
			std::optional<Collision> newCollision = newCollisions.getCollision(entity1, entity2); 
            if (newCollision) {
				continue;
			}
            if(savedCollision.state == CollisionState::Entered || savedCollision.state == CollisionState::Lasting) {
                _savedCollisions.updateCollision(entity1, entity2, Collision::exitedCollision());
            } else if(savedCollision.state == CollisionState::Exited) {
                _savedCollisions.removeCollision(entity1, entity2);
            }
        }
    }
}

void CollisionSystem::_readCurrentCollisions(const CollisionGraph& newCollisions) {
    for(auto& entity1 : newCollisions.getCollidingEntities()) {
        for(auto& [entity2, newCollision] : newCollisions.getCollisions(entity1)) {
			std::optional<Collision> savedCollision = _savedCollisions.getCollision(entity1, entity2);
            if (!savedCollision) {
				_savedCollisions.addCollision(
                    entity1,
                    entity2,
                    newCollision.changeState(CollisionState::Entered)
                );
				continue;
            }
			CollisionState savedCollisionState = savedCollision.value().state;
            switch(savedCollisionState){
                case CollisionState::Entered:
                    _savedCollisions.updateCollision(
                        entity1,
                        entity2,
                        newCollision.changeState(CollisionState::Lasting)
                    );
                    break;
                case CollisionState::Exited:
                    _savedCollisions.updateCollision(
                        entity1,
                        entity2,
                        newCollision.changeState(CollisionState::Entered)
                    );
                    break;
				case CollisionState::Lasting:
					_savedCollisions.updateCollision(
                        entity1,
                        entity2,
                        newCollision.changeState(CollisionState::Lasting)
                    );
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
        std::optional<CollisionState> state = _mouseCollisions.getCollision(entity);
		std::optional<CollisionState> newState = newMouseCollisions.getCollision(entity);
        if (!state || newState) {
			continue;
        }
		if (state.value() == CollisionState::Entered || state.value() == CollisionState::Lasting) {
			_mouseCollisions.updateCollision(entity, CollisionState::Exited);
		} else if (state.value() == CollisionState::Exited) {
			_mouseCollisions.removeCollision(entity);
		}
    }
}

void CollisionSystem::_readCurrentMouseCollisions(const MouseSet& newMouseCollisions) {
    for(auto& entity : newMouseCollisions.getMouseEntities()) {
        std::optional<CollisionState> state = _mouseCollisions.getCollision(entity);
        if (!state) {
			_mouseCollisions.addCollision(entity, CollisionState::Entered);
			continue;
        }
        switch(state.value()){
            case CollisionState::Entered:
                _mouseCollisions.updateCollision(entity, CollisionState::Lasting);
                break;
            case CollisionState::Exited:
                _mouseCollisions.updateCollision(entity, CollisionState::Entered);
                break;
            case CollisionState::Lasting:
                break;
            case CollisionState::CurrentlyFound:
                throw PhysicsException("Saved mouse collision's state should not be CurrentlyFound");
        }
    }
}

bool CollisionSystem::hasMouseEntered(ecs::Entity entity) const {
    return _mouseCollisions.getCollision(entity) == CollisionState::Entered;
}

bool CollisionSystem::hasMouse(ecs::Entity entity) const {
	std::optional<CollisionState> state = _mouseCollisions.getCollision(entity);
    if (!state) {
		return false;
    }
    return state.value() == CollisionState::Entered || state.value() == CollisionState::Lasting;
}

bool CollisionSystem::hasMouseExited(ecs::Entity entity) const {
	std::optional<CollisionState> state = _mouseCollisions.getCollision(entity);
    return state && state.value() == CollisionState::Exited;
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