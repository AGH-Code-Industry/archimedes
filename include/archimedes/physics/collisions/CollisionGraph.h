#pragma once
#include <archimedes/ecs/Domain.h>
#include <archimedes/physics/collisions/Collision.h>
#include <archimedes/ecs/ComponentPool.h>

namespace arch::physics {

/// @brief Stores all collision data in a directed graph
class CollisionGraph {
    ecs::ComponentPool<ecs::ComponentPool<Collision>> _graph;

    public:

    ///@brief Get all collisions that a given entity has
    OptRef<const ecs::ComponentPool<Collision>> getCollisions(ecs::Entity entity) const; 
    
    /// @brief Get all entities that collide with at least one entity
    std::vector<ecs::Entity> getCollidingEntities() const;

    /// @brief Graph CRUD operations
    void addCollision(ecs::Entity entity1, ecs::Entity entity2, const Collision& collision);
    void updateCollision(ecs::Entity entity1, ecs::Entity entity2, const Collision& collision);
    void removeCollision(ecs::Entity entity1, ecs::Entity entity2);
    std::optional<Collision> getCollision(ecs::Entity entity1, ecs::Entity entity2) const;

};

} // namespace arch::physics