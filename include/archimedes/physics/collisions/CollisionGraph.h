#pragma once
#include <archimedes/ecs/Domain.h>
#include <archimedes/physics/collisions/Collision.h>
#include <unordered_map>

namespace arch::physics {

class CollisionGraph {
    std::unordered_map<ecs::Entity, std::unordered_map<ecs::Entity, Collision>> _graph;

    public:

    std::unordered_map<ecs::Entity, Collision> getCollisions(ecs::Entity entity) const;  
    std::vector<ecs::Entity> getCollidingEntities() const;
    void addCollision(ecs::Entity entity1, ecs::Entity entity2, const Collision& collision);
    void updateCollision(ecs::Entity entity1, ecs::Entity entity2, const Collision& collision);
    void removeCollision(ecs::Entity entity1, ecs::Entity entity2);
    std::optional<Collision> getCollision(ecs::Entity entity1, ecs::Entity entity2) const;

};

} // namespace arch::physics