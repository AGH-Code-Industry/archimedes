#pragma once
#include <archimedes/ecs/Domain.h>
#include <archimedes/scene/components/TransformComponent.h>


namespace arch::physics {
using TransformComponent = arch::scene::components::TransformComponent;
using CollisionSet = std::unordered_set<std::pair<ecs::Entity, ecs::Entity>, std::hash<std::pair<ecs::Entity, ecs::Entity>>>;

enum CollisionState {
    Entered,
    Exited,
    Lasting,
    NotExisting
};

class CollisionSystem {
    ecs::Domain& _domain;

    CollisionSet _enteredCollisions;
	CollisionSet _exitedCollisions;
	CollisionSet _lastingCollisions;
    
    CollisionSet _getCollidableEntities() const;
    void _addCollision(ecs::Entity entity1, ecs::Entity entity2, CollisionState state);
    void _moveCollision(ecs::Entity entity1, ecs::Entity entity2, CollisionState fromState, CollisionState toState);
    void _removeCollision(ecs::Entity entity1, ecs::Entity entity2);
    void _removeStaleCollisions(const CollisionSet& newCollisions);
    void _saveExitedCollisions(const CollisionSet& newCollisions);
    void _readCurrentCollisions(const CollisionSet& newCollisions);
    CollisionState _getCollisionState(ecs::Entity entity1, ecs::Entity entity2) const;

public:
    explicit CollisionSystem(ecs::Domain& domain);
    std::vector<ecs::Entity> getEnteredCollisions(ecs::Entity entity) const;
	std::vector<ecs::Entity> getExitedCollisions(ecs::Entity entity) const;
	std::vector<ecs::Entity> getlastingCollisions(ecs::Entity entity) const;
    void update();
};
}