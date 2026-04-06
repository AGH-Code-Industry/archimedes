#pragma once
#include <archimedes/ecs/Domain.h>
#include <archimedes/physics/collisions/CollisionState.h>
#include <unordered_map>

namespace arch::physics {

class MouseSet {
    std::unordered_map<ecs::Entity, CollisionState> _set;

    public:

    void addCollision(ecs::Entity entity, CollisionState state);
    void changeCollisionState(ecs::Entity entity, CollisionState toState);
    void removeCollision(ecs::Entity entity);
    std::vector<ecs::Entity> getMouseEntities() const;
    CollisionState getCollisionState(ecs::Entity entity) const;

};

} // namespace arch::physics