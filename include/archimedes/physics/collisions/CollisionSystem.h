#pragma once
#include <archimedes/ecs/Domain.h>
#include <archimedes/physics/collisions/CollisionGraph.h>
#include <archimedes/physics/collisions/MouseSet.h>

namespace arch::physics {

class CollisionSystem {
    ecs::Domain& _domain;
    CollisionGraph _savedCollisions;
    MouseSet _mouseCollisions;

    CollisionGraph _getCollidedEntities() const;
    void _checkDisappearedCollisions(const CollisionGraph& newCollisions);
    void _readCurrentCollisions(const CollisionGraph& newCollisions);

    MouseSet _getMouseEntities(float3 mousePosition) const;
    void _checkDisappearedMouseCollisions(const MouseSet& newMouseCollisions);
    void _readCurrentMouseCollisions(const MouseSet& newMouseCollisions);

public:
    explicit CollisionSystem(ecs::Domain& domain);
    std::vector<ecs::Entity> getEnteredCollisions(ecs::Entity entity) const;
    std::vector<ecs::Entity> getCollisions(ecs::Entity entity) const;
	std::vector<ecs::Entity> getExitedCollisions(ecs::Entity entity) const;

    bool hasMouseEntered(ecs::Entity entity) const;
    bool hasMouse(ecs::Entity entity) const;
    bool hasMouseExited(ecs::Entity entity) const;

    void update(float3 mousePosition);
};
}