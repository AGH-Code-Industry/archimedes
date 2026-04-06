#pragma once
#include <archimedes/ecs/Domain.h>
#include <unordered_map>

namespace arch::physics {

enum CollisionState {
    Entered,
    Exited,
    Lasting,
    NotExisting,
    CurrentlyFound
};


class CollisionGraph {
    std::unordered_map<ecs::Entity, std::unordered_map<ecs::Entity, CollisionState>> _graph;

    public:

    std::unordered_map<ecs::Entity, CollisionState> getCollisions(ecs::Entity entity) const;  
    std::vector<ecs::Entity> getCollidingEntities() const;
    void addCollision(ecs::Entity entity1, ecs::Entity entity2, CollisionState state);
    void changeCollisionState(ecs::Entity entity1, ecs::Entity entity2, CollisionState toState);
    void removeCollision(ecs::Entity entity1, ecs::Entity entity2);
    CollisionState getCollisionState(ecs::Entity entity1, ecs::Entity entity2) const;

};

} // namespace arch::physics